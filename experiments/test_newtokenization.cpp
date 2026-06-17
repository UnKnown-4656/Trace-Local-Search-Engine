#include <iostream>
#include <string>
#include <cctype>
#include <vector>


using namespace std;



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

int main()
{
    string target = "CamelCase_with_Normal_Tokenization.pdf";
    string current;
    vector <string> tokens ;
    for(char &c : target){
        if(c=='_' || c=='.' || c=='-' || c==' ')
        {
            //if(isupper(c)){
            if(!current.empty()){
                tokens.push_back(current);
                current.clear();

            }
            //}
        }
        else{
        current+=c;
        }
    }
    
    vector<string> results;
    for(auto token :tokens){
        //cout <<token <<endl;
        vector <string >Results=camelcase(token);
        for(const string&res :Results){
            results.push_back(res);
        }
    }
    for(const auto &res:results){
        cout <<res<<endl;
    }

  

}