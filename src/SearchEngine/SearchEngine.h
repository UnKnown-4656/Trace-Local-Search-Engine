#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm> // Used to get .sort function
#include "../Utils/utils.h"


struct SearchResult
{
    std::string path;
    int score;
};

class SearchEngine{
private:
    const std::unordered_map<std::string,std::unordered_set<std::string>>& files;
    //vector<string> Tokenize(string& str);
public:
    SearchEngine(const std::unordered_map<std::string ,std::unordered_set<std::string>>& files);
    std::vector <SearchResult>search(std::string target);   
    void searchwithintersection(std::string target);
    
};