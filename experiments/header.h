#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>

namespace fs = std::filesystem;
using namespace std;

string ToLower(string str);
vector<string> Tokenize(string &str);
void ScanFiles(fs::path Path ,unordered_multimap<string ,string >& files);
void SerchFunction(unordered_multimap<string ,string >& files,string Target);
string input(string placeholder);
void SaveIndex(unordered_multimap<string, string> &files, string filename);
void LoadIndex(unordered_multimap<string, string> &files, string filename);
