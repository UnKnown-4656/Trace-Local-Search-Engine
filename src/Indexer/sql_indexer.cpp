#include "sql_indexer.h"

vector<string> Indexer::Tokenize(string &str) {
    vector<string> tokens;
    string current;
    for (char c :str){
        c=tolower(c);
        if(c=='_' || c=='.' || c=='-' || c==' ' || c=='(' || c==')' || c== '[' || c==']'){
            if(!current.empty()){  //Not Empty
                tokens.push_back(current); //push current  
                current.clear();
            }
        }
        else{
            current+=c;
        }
    }
    if(!current.empty()){
        tokens.push_back(current);
    }
    return tokens;
}
string Indexer::sql_q(string table_name ,string file_name ,string path,int score)
{
    return "INSERT INTO " + table_name + " VALUES('" + file_name +"','" + path + "'," + to_string(score) + ");" ;
    
}

int Indexer::callback(
        void*data, 
        int ColumnCount, // means how many column 
        char** RowsValues, //row data ** because multiple strings can be stored 
        char** columnNames // colunmNames  ** because multiple string can be stored 
)
    {
        for(int i =0; i <ColumnCount;i++) //printing data
        {
            cout <<columnNames[i]
                <<" : "
                <<RowsValues[i]
                <<endl;
        } 
        
        cout << "----------------\n";
        return 0;
    }

void Indexer::ScanFiles(fs::path Path){

        try
    {
        for (const auto &entry : fs::directory_iterator(Path))
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
        return;
    }

}
void Indexer::save_index(string file_path ){
    cout <<"Testing Sqlite...."<<endl;
    sqlite3* db;

    int result =
        sqlite3_open("indexing.db",&db);
    const char * create_table =
        "CREATE TABLE IF NOT EXISTS file_index("
        "token TEXT,"
        "file_paths TEXT,"
        "score INTEGER"
        ");";
    const char *view_table=
        "SELECT * FROM file_index;";

    string add_table =add_value("file_index","Dev","Path",50);


}
