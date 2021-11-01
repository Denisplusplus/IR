#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "search.h"
#include "query.h"


const std::string indexes_path = "/Users/denis/MAI/IR/lab3/wikipedia_spimi_indexes";


std::vector<unsigned long long> OperationAND(std::vector<unsigned long long> v1, std::vector<unsigned long long> v2) {
    std::vector<unsigned long long > v3;

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          std::back_inserter(v3));
	return v3;
}

std::vector<unsigned long long> OperationOR(std::vector<unsigned long long> v1, std::vector<unsigned long long> v2) {
    std::vector<unsigned long long > v3;

    std::set<unsigned long long> s1(v1.begin(), v1.end());
    std::set<unsigned long long> s2(v2.begin(), v2.end());

    std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(v3, v3.begin()));
	return v3;
}



std::vector<unsigned long long> SearchInIndex(std::stack<Token> query_stack, std::string index_path ) {

    TSearcher searcher = TSearcher(index_path);

    std::string term1 = "";
    std::string term2 = "";
    std::vector<unsigned long long> result;
    std::vector<unsigned long long> res1; 
    std::vector<unsigned long long> res2;

    int query_stack_size = query_stack.size();
    
    while(!query_stack.empty()) {

        Token token = query_stack.top();
        query_stack.pop();

        if (token.value != "") {
            if (term1 == "") {
                term1 = token.value;
                if (token.operation == "PROCESS") {
                    res1 = result; 
                } else {
                    res1 = searcher.Search(term1);
                }
            } else if (term2 == "") {
                term2 = token.value;
                if (token.operation == "PROCESS") {
                    res2 = result; 
                } else {
                    res2 = searcher.Search(term2);
                }
            }
        } else if (token.operation != "") {

            if (token.operation == "&&") {
                result = OperationAND(res1, res2);
            }

            if (token.operation == "||") {
                result = OperationOR(res1, res2);
            }

            term1 = "";
            term2 = "";

            query_stack.push(Token{"PROCESS", "PROCESS", BINARY});
        }
    }
    if (query_stack_size == 1) {
        result = res1;
    }
    return result;
}


void SearchAllIndexes(std::stack<Token> query_stack) {
    std::cout << "searching..." << std::endl;
    std::chrono::steady_clock::time_point time_clock_begin, time_clock_end;

    std::vector<unsigned long long> results;

    time_clock_begin = std::chrono::steady_clock::now();
    for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(indexes_path), {})) { ;
        std::vector<unsigned long long> res = SearchInIndex(query_stack, entry.path().string());
        std::copy(res.begin(), res.end(), back_inserter(results));
	}
    std::sort(results.begin(), results.end()); 

    std::cout << std::endl << "Result: " << results.size() << " records" << std::endl;
    time_clock_end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(time_clock_end - time_clock_begin).count() << "[sec]" << std::endl;

    for (unsigned long long i = 0; i < results.size(); i++) {
       std::cout << "№" << i+1 << " | doc_id: " << results[i] << " | link: https://ru.wikipedia.org/?curid=" << results[i] << std::endl;
    }
}


void menu(void)
{   
    std::cout << std::endl << std::endl;
	std::cout << "Menu:" << std::endl;
	std::cout << "1) Search" << std::endl;
	std::cout << "2) Print this menu" << std::endl;
	std::cout << "0) Exit" << std::endl;
}

int main() {
    std::string query, queryPreprocessed;
    std::stack<Token> query_stack;

    std::string action;
    menu();
    while (1) {
        std::cin >> action;
        
        if (action == "1") {
            std::cout << "Input a query" << std::endl;
            std::cin >> std::ws;
            std::getline(std::cin, query);

            std::string queryPreprocessed = PreprocessQuery(query);
            std::stack<Token> query_stack = ToPostfix(queryPreprocessed);
            SearchAllIndexes(query_stack);
            menu();
        } else if (action == "2") {
            menu();
        } else if (action == "0") {
          return 0;
        } else {
            std::cout << "wrong action" << std::endl;
            menu();
        }
    }
    
    return 0;
}