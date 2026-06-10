#include <iostream>
#include <string>


using namespace std;

string add_value(string table_name ,string file_name ,string path,int score)
{
    return "INSERT INTO " + table_name + " VALUES('" + file_name +"','" + path + "'," + to_string(score) + ");" ;
    
}

int main()
{   
    cout <<add_value("test","test.txt","path", 50);


    return 0;
}