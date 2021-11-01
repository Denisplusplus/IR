#ifndef TINDEXER
#define TINDEXER

#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>


class TIndexer
{
public:
    TIndexer() {};
    void BuildIndex(std::string input_articles_path, std::string output_indexes_path, unsigned long long batch_size);
    void Load(std::string index_path);
private:
    void WriteToIndex(std::string wiki_doc_path);
    bool InitBuilder(std::string input_articles_path, std::string output_indexes_path, unsigned long long batch_size);
    void Save(std::string counter);
    unsigned long long batch_size;
    std::string input_articles_path;
    std::string output_indexes_path;

    std::map<std::string, std::vector<unsigned long long>> index;
};

#endif // TINDEXER
