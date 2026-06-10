#include <iostream>
#include <string>
#include "sql/sqlite3.h"
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;


class Indexer
{
    private:
        unordered_map<string ,unordered_set<string>>files;
        vector<string> Tokenize(string &str);
    public: 
        int callback(
            void*data, 
            int ColumnCount, // means how many column 
            char** RowsValues, //row data ** because multiple strings can be stored 
            char** columnNames // colunmNames  ** because multiple string can be stored 
        );
        void ScanFiles(fs::path Path);
        void save_index();
    
};
