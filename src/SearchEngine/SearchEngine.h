#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

struct SearchResult
{
    string path;
    int score;
};

class SearchEngine{
private:
    const unordered_map<string,unordered_set<string>>& files;
    vector<string> Tokenize(string& str);
public:
    SearchEngine(const unordered_map<string ,unordered_set<string>>& files);
    vector <SearchResult>search(string target);   
    void searchwithintersection(string target);
    
};