#include "sql_indexer.h"

using namespace std;

// vector<string> Indexer::Tokenize(const string &str) {
//     vector<string> tokens;
//     string current;
//     for (char c :str){
//         c = static_cast<char>(tolower(static_cast<unsigned char>(c)));;
//         if(c=='_' || c=='.' || c=='-' || c==' ' || c=='(' || c==')' || c== '[' || c==']'){
//             if(!current.empty()){  //Not Empty
//                 tokens.push_back(current); //push current  
//                 current.clear();
//             }
//         }
//         else{
//             current+=c;
//         }
//     }
//     if(!current.empty()){
//         tokens.push_back(current);
//     }
//     return tokens;
// }

int Indexer::callback(
        //unordered_map <string,string> data
        void*data, 
        int ColumnCount, // means how many column 
        char** RowsValues, //row data ** because multiple strings can be stored 
        char** columnNames // colunmNames  ** because multiple string can be stored 
    )
    {
        
            //cout <<columnNames[i]
                //<<" : "
                //<<RowsValues[i]
                //<<endl;
        auto* files =
            static_cast<unordered_map<string,unordered_set<string>>*>(data);
        if(RowsValues[0] && RowsValues[1]){
            string token =RowsValues[0];
            string path  =RowsValues[1];
            (*files)[token].insert(path);
        }
        
        ///cout << "----------------\n";
        return 0;
    }

void Indexer::ScanFiles(fs::path Path){

        try
    {
        for (const auto &entry : fs::directory_iterator(Path,fs::directory_options::skip_permission_denied))
        {
            if (entry.is_directory())
            {
                ScanFiles(entry.path());
            }
            if (entry.is_regular_file())
            {
                string filename = entry.path().filename().string();
                //string LowerFilename = ToLower(filename);
                string path = entry.path().string();
                vector<string> tokenized_filename = Tokenize(filename);
            // cout <<entry.path()<<endl;
                for(const auto &token : tokenized_filename){
                    files[token].insert(path);
                }
                //files.insert({ToLower(entry.path().filename().string()), entry.path().string()});
            }
        }
    }
    catch (exception &e)
    {
        cout << "Error : " << e.what() << endl;

        //continue;
    
    }

}
void Indexer::save_index(const string fileName){
    //cout <<"Testing Sqlite...."<<endl;

    sqlite3* db;

    int result =
        sqlite3_open(fileName.c_str(),&db);

    if(result != SQLITE_OK)
    {
        cout << "Database open failed\n";
        return;
    }

    char * errMsg = nullptr;
    
    const char * create_table =
        //"DELETE FROM file_index;"
        "CREATE TABLE IF NOT EXISTS file_index("
        "token TEXT,"
        "file_paths TEXT ,"
        "UNIQUE(token,file_paths)"
        ");"
        "DELETE FROM file_index;";

    result=sqlite3_exec(
        db,
        create_table,
        nullptr,
        nullptr,
        &errMsg
    );
    if(result != SQLITE_OK){
        cout << "[CREATE ERROR]" <<errMsg  <<endl;
    }


    sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr);
    const char* insert_sql = "INSERT OR IGNORE INTO file_index(token, file_paths) VALUES(?, ?);"; //Setting insert command with ' ? ' place Holder
    sqlite3_stmt* stmt; 

    ///sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr); //Prepare insert query 
    if(sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    for (const auto& entry : files) {
        for (const auto& path : entry.second) {
            sqlite3_bind_text(stmt, 1, entry.first.c_str(), -1, SQLITE_STATIC); // Binding text to place holder ,converting c++ string to c char * using .c_str
            sqlite3_bind_text(stmt, 2, path.c_str(),        -1, SQLITE_STATIC); //same
            //sqlite3_step(stmt);// Add
            int rc = sqlite3_step(stmt);

            if(rc != SQLITE_DONE)
            {
                cout << sqlite3_errmsg(db) << endl;
            }

            sqlite3_reset(stmt); // Reset
            sqlite3_clear_bindings(stmt); //Clear bindings
        }
    }

    sqlite3_finalize(stmt); // Finalize

    // for (const auto &entry : files){
    //     for(const auto &path : entry.second){
    //         string add_table =sql_q("file_index",entry.first,path);
    //         result=sqlite3_exec(
    //             db,
                
    //             add_table.c_str(),
    //             nullptr,
    //             nullptr,
    //             &errMsg
    //             );
    //         if(result != SQLITE_OK){
    //             cout << "[ADD ERROR]" <<errMsg  <<endl;
    //             }

            
    //     }

    // }
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr); 
    sqlite3_close(db);

}


void Indexer::load_index(string fileName){
    files.clear();
    sqlite3* db;
    int result =
    sqlite3_open(fileName.c_str(),&db);

    if(result != SQLITE_OK)
    {
        cout << "Database open failed\n";
        return;
    }
    char *errMsg =nullptr;
    const char * view_index =
        "SELECT * FROM file_index";

    result=sqlite3_exec(
        db,
        view_index,
        callback,
        &files,
        &errMsg
    );
    if(result != SQLITE_OK)
    {
        cout << "[LOAD ERROR] "
            << errMsg
            << endl;
    }

    sqlite3_close(db);

}
const unordered_map<string, unordered_set<string>>& Indexer::getFiles() const {
    return files;
}
