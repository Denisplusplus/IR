#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <string>
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <chrono>
#include <map>
#include <filesystem>

#include "search.h"



TSearcher::TSearcher(std::string index_path) {
	if (!boost::filesystem::exists(index_path)) {
		std::cout << "input file not found" << std::endl;
		return;
	}

	this->index_path = index_path;
	this->LoadIndex();
	std::cout << "Loaded index " << index_path << std::endl;
}


std::vector<unsigned long long> TSearcher::Search(std::string term) {
	auto it = this->index.find(term);
	if (it == this->index.end()) {
		return std::vector<unsigned long long>{};
	}
	return this->index[term];
}


void TSearcher::LoadIndex() {
	std::ifstream f(this->index_path, std::ios::binary);
	if (f.fail()) {
		std::cout << "error" << std::endl;
		return;
	}
	boost::archive::binary_iarchive ia(f);
	ia >> this->index;
}
