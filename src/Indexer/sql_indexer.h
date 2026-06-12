#pragma once

#include <iostream>
#include <string>
#include "sql/sqlite3.h"
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include "../Utils/utils.h"

namespace fs = std::filesystem;

class Indexer
{
    private:
        //unordered_map<string ,string>data;
        std::unordered_map<std::string ,std::unordered_set<std::string>>files;
        //vector<string> Tokenize(const string &str);
    public: 
        static int callback(
            void*data, 
            int ColumnCount, // means how many column 
            char** RowsValues, //row data ** because multiple strings can be stored 
            char** columnNames // colunmNames  ** because multiple string can be stored 
        );
        void ScanFiles(fs::path Path);
        void save_index(const std::string fileName);
        void load_index(std::string fileName);
        const std::unordered_map<std::string, std::unordered_set<std::string>>& getFiles() const;

    
};
