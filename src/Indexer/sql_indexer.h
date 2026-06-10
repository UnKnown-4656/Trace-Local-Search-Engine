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
        string sql_q(string table_name ,string file_name ,string path,int score);
        int callback(
            void*data, 
            int ColumnCount, // means how many column 
            char** RowsValues, //row data ** because multiple strings can be stored 
            char** columnNames // colunmNames  ** because multiple string can be stored 
        );
        void scan(string path);
        void save_index(string file_name);
    

}
