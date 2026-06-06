#include "includes/header.h"
#include <vector>

using namespace std;

vector<string> Tokenize(string &str){
    vector<string> tokens;
    string current;
    for (char c :str){
        c=tolower(c);
        if(c=='_' || c=='.' || c=='-' || c==' '){
            if(!current.empty()){
                tokens.push_back(current);
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
int main(){
    string str = "HELLO_WORLD";
    //vector<string> Tokenize(string text);
    
    vector<string> result=Tokenize(str);
    for (const auto &token : result){ 
        cout << token << endl;
    }

    return 0;
}