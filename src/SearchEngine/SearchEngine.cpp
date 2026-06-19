#include "SearchEngine.h"
using namespace std;

// ✅ Clean constructor, no stray braces
SearchEngine::SearchEngine(const unordered_map<string, unordered_set<string>>* files)
    : files(files) {}

vector<SearchResult> SearchEngine::search(const string& Target) {  // ✅ const ref
    vector<string> tokenized_target = Tokenize(Target);
    unordered_map<string, int> scores;
    vector<SearchResult> results;

    try {
        for (const auto& token : tokenized_target) {
            auto it = files->find(token);          // ✅ -> not .
            if (it != files->end()) {              // ✅ -> not .
                for (const auto& path : it->second) {
                    scores[path]++;
                }
            }
        }

        if (scores.empty()) {
            cout << "File Not Found" << endl;
            return {};
        }

        for (const auto& item : scores) {
            results.push_back({item.first, item.second});
        }

        sort(results.begin(), results.end(),
            [](const SearchResult& a, const SearchResult& b) {
                return a.score > b.score;
            });

        return results;
    }
    catch (exception& e) {
        cout << "Error : " << e.what() << endl;
        return {};
    }
}

void SearchEngine::searchwithintersection(const string& target) {  // ✅ const ref
    try {
        vector<string> tokenized_target = Tokenize(target);
        unordered_set<string> result_set = files->at(tokenized_target[0]); // ✅ ->
        
        for (const auto& token : tokenized_target) {
            unordered_set<string> current_set = files->at(token);          // ✅ ->
            unordered_set<string> temp_set;
            
            for (const auto& item : result_set) {
                if (current_set.find(item) != current_set.end()) {
                    temp_set.insert(item);
                }
            }
            result_set = temp_set;
        }

        if (result_set.empty()) {
            cout << "File Not Found...." << endl;
        } else {
            for (const auto& item : result_set) {
                cout << item << endl;
            }
        }
    }
    catch (exception& e) {
        cout << "Files Not Found...." << endl;
        return;
    }
}