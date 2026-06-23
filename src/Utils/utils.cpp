#include "utils.h"

using namespace std;

string ToLower(string str){
    //for (char c :str){ // works but makes copy 
    //    c=tolower(c);
    //}
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}
//string ToLower(string target)
vector <string>camelcase(string &token){
    string current;
    vector <string> tokens;
    for(char c : token){
        if(isupper(c)){
            if(!current.empty())
            {
                tokens.push_back(current);
                current.clear();

            }
        }
        current+=c;
    }
    if(!current.empty()){
        tokens.push_back(current);
        current.clear();
    }

    return tokens;
}

vector<string>Tokenize(const string &str) {
    vector<string> tokens;
    string current;
    for (char c :str){
        //c = static_cast<char>(tolower(static_cast<unsigned char>(c)));;
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
    //return tokens;   
    vector<string> results;
    for(auto token :tokens){
        //cout <<token <<endl;
        vector <string >Results=camelcase(token);
        for(const string&res :Results){
            string low_res=ToLower(res);
            // Have to LowerCase Before pushing 
            results.push_back(low_res);
        }
    }
    //for(const auto &res:results){
     //   cout <<res<<endl;
    //}
    return results;

}

string input(string placeholder)
{
    string str;
    cout << placeholder;
    cin.ignore(); // Clear the input buffer
    getline(cin, str); //will read the whole line even if it contains spaces
    return str;
}