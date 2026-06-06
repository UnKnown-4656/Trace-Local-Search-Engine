#include <iostream>
#include "sqlite3.h"

using namespace std;
int callback(
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

int main()
{
    sqlite3* db;

    int result =
        sqlite3_open("indexing.db",&db);

    const char * create_table= 
        "CREATE TABLE IF NOT EXISTS file_index("
        "token TEXT,"
        "file_paths TEXT,"
        "score INTEGER"
        ");";

    
    const char * view_table=
        "SELECT * FROM file_index;";
    const char * add_table =
        "INSERT INTO index "
        "VALUES('test.txt','D//:downloads/test.txt', 2);";

    char * errMsg = nullptr;

    result=sqlite3_exec(
        db,
        create_table,
        nullptr,
        nullptr,
        &errMsg
    );
    if(result == SQLITE_OK){
        cout << " \n";
    }
    else{
        cout << "[CREATE ERROR]" <<errMsg  <<endl;
    }

    result=sqlite3_exec(
        db,
        view_table,
        callback,
        nullptr,
        &errMsg

    );
    if(result == SQLITE_OK){
        cout << " \n";
    }
    else{
        cout << "[VIEW ERROR]" <<errMsg  <<endl;
    }



    //i//f(result==SQLITE_OK){
     //   cout << "Table Created and query Executed\n";

   // }   
    //else{
       // cout << errMsg;
    //}
    sqlite3_close(db);

    return 0;    
}