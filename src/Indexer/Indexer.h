#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;
namespace fs = std::filesystem;

class Indexer
{
private:
    unordered_map<string ,unordered_set<string>>files;
    vector<string> Tokenize(string &str);
public:
    void ScanFiles(fs::path Path);
    void SaveIndex(string filename);
    void LoadIndex(string filename);
    const unordered_map<string, unordered_set<string>>& getFiles() const;


};
