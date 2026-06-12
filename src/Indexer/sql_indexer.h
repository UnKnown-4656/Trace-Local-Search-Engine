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


class Indexer
{
    private:
        //unordered_map<string ,string>data;
        unordered_map<string ,unordered_set<string>>files;
        vector<string> Tokenize(const string &str);
    public: 
        static int callback(
            void*data, 
            int ColumnCount, // means how many column 
            char** RowsValues, //row data ** because multiple strings can be stored 
            char** columnNames // colunmNames  ** because multiple string can be stored 
        );
        void ScanFiles(fs::path Path);
        void save_index(const string fileName);
        void load_index(string fileName);
        const unordered_map<string, unordered_set<string>>& getFiles() const;

    
};
