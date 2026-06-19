#pragma once
#include <string>
// REMOVED: duplicate #include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "../Utils/utils.h"

struct SearchResult {
    std::string path;
    int score;
};

class SearchEngine {
private:
    // ✅ Already correct — pointer, good
    const std::unordered_map<std::string, std::unordered_set<std::string>>* files;

public:
    // ❌ Was: const map& files  (reference)
    // ✅ Now: const map* files  (pointer, matches the member above)
    SearchEngine(const std::unordered_map<std::string, std::unordered_set<std::string>>* files);

    // ❌ Was: std::string target  (copies the string every call)
    // ✅ Now: const std::string&  (read-only, zero copy)
    std::vector<SearchResult> search(const std::string& target);

    // ❌ Was: std::string target  (copies the string every call)
    // ✅ Now: const std::string&  (read-only, zero copy)
    void searchwithintersection(const std::string& target);
};