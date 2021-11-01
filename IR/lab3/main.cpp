#include <string>
#include <iostream>

#include "indexer.h"



int main()
{
    std::string raw_data_folder = "/Users/denis/MAI/IR/lab3/wikipedia_articles_tokenized";
    std::string output_index_folder = "/Users/denis/MAI/IR/lab3/wikipedia_indexes";

    unsigned long long batch_size = 1000; 
    
    TIndexer indexer = TIndexer();
    indexer.BuildIndex(raw_data_folder, output_index_folder, batch_size);
    return 0;
}