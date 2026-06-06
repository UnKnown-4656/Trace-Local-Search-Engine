#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

string ToLower(string str);
vector<string> Tokenize(string &str);
void ScanFiles(fs::path Path ,unordered_map<string ,unordered_set<string>> & files);
void Intersection(unordered_map<string , unordered_set<string>>&files ,string Target);
void SerchFunction(unordered_map<string, unordered_set<string>> &files,string Target);
string input(string placeholder);
void SaveIndex(unordered_map<string, unordered_set<string>> &files, string filename);
void LoadIndex(unordered_map<string, unordered_set<string>> &files, string filename);
