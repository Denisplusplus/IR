#ifndef TSEARCHER
#define TSEARCHER

#include <iostream>
#include <vector>
#include <map>


class TSearcher
{
public:
    TSearcher(std::string index_path);
    std::vector<unsigned long long> Search(std::string term);
private:
    void LoadIndex();
    std::string index_path;
    std::map<std::string, std::vector<unsigned long long>> index;
};

#endif // TSEARCHER
