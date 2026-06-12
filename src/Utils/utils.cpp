#include "utils.h"

using namespace std;
vector<string>Tokenize(const string &str) {
    vector<string> tokens;
    string current;
    for (char c :str){
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));;
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

string input(string placeholder)
{
    string str;
    cout << placeholder;
    cin.ignore(); // Clear the input buffer
    getline(cin, str); //will read the whole line even if it contains spaces
    return str;
}