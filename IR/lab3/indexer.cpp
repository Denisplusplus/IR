#include <iostream>
#include <sstream>
#include <chrono>
#include <map>
#include <filesystem>
#include <string>
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "nlohmann/json.hpp"
#include "indexer.h"


bool FilesystemCreateFolderIdempotent(std::string path) {
	boost::filesystem::path dir(path);
	
	if (!(boost::filesystem::exists(dir))) {
		return boost::filesystem::create_directory(dir);
	}
	return true;
}


bool TIndexer::InitBuilder(std::string input_articles_path, std::string output_indexes_path, unsigned long long batch_size) {
	if (!boost::filesystem::exists(input_articles_path)) {
		std::cout << "input directory not found" << std::endl;
		return false;
	}

 	if (boost::filesystem::is_empty(input_articles_path)) {
        std::cout << "input directory is empty" << std::endl; 
		return false;
    }

	if (!FilesystemCreateFolderIdempotent(output_indexes_path)) {
		std::cout << "cannot create output indexes dir" << std::endl;
		return false;
	}

	this->input_articles_path = input_articles_path;
	this->output_indexes_path = output_indexes_path;
	this->batch_size = batch_size;
	this->index.clear();

	return true;
}


void TIndexer::WriteToIndex(std::string wiki_doc_path) {
	std::ifstream wiki_article(wiki_doc_path);
	nlohmann::json wiki_doc = nlohmann::json::parse(wiki_article);
	std::string doc_id_str = wiki_doc["id"];

	unsigned long long wiki_doc_id = std::stoi(doc_id_str);
	
	std::string tokenized_text = wiki_doc["text"];
	std::istringstream token_stream(tokenized_text);
	std::string token;

	while(token_stream >> token) {
		auto index_iterator = this->index.find(token);
		if (index_iterator == this->index.end()){
			this->index[token] = std::vector<unsigned long long >{wiki_doc_id};
		} else {
			if (index_iterator->second.back() != wiki_doc_id) {
				index_iterator->second.emplace_back(wiki_doc_id);
			}
		}
	}
}


void TIndexer::BuildIndex(std::string input_articles_path, std::string output_indexes_path, unsigned long long batch_size) {
	if (!this->InitBuilder(input_articles_path, output_indexes_path, batch_size)) {
		return;
	}

	// statistics variables
	unsigned long long articles_counter = 0;
	unsigned long long articles_counter_general = 0;
	std::chrono::steady_clock::time_point time_clock_begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point time_clock_end;


	for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(this->input_articles_path), {})) {
		articles_counter++;
		articles_counter_general++;

		if (articles_counter < this->batch_size) {
			this->WriteToIndex(entry.path().string());
		} else {
			for (auto& arr : this->index) {
				std::sort(arr.second.begin(), arr.second.end());
			}
			this->Save(std::to_string(articles_counter_general));

			time_clock_end = std::chrono::steady_clock::now();
			std::cout << "exec time for batch = " <<std::chrono::duration_cast<std::chrono::minutes>(time_clock_end - time_clock_begin).count() << "[m]" << std::endl;

			articles_counter = 0;
			time_clock_begin = std::chrono::steady_clock::now();
			this->index.clear();
			this->WriteToIndex(entry.path().string());
		}
	}
	if (this->index.size() > 0) {
		this->Save(std::to_string(articles_counter_general));
	}
}


void TIndexer::Save(std::string postfix)
{
	std::cout << this->output_indexes_path << std::endl;

	std::string index_path = this->output_indexes_path + "/wiki_articles_" + postfix + ".index";
	std::ofstream f(index_path, std::ios::binary);
	if (f.fail()) {
		std::cout << "error" << std::endl;
		return;
	}

	boost::archive::binary_oarchive oa(f);

	auto indexValue = this->index;
	oa << indexValue;
}

void  TIndexer::Load(std::string index_path) {
	this->index.clear();
	std::ifstream f(index_path, std::ios::binary);
	if (f.fail()) {
		std::cout << "error while load data in index" << std::endl;
		return;
	}
	boost::archive::binary_iarchive ia(f);
	ia >> this->index;
}
