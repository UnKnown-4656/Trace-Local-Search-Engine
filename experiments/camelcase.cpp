#include <iostream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

int main()
{
    string camel = "CamelCase";
    string current;
    vector<string> tokens;


    for(char c :camel){
        if(isupper(c)) // step 1: First Letter C = c  step 3: Found an upper case "C" and the previous current goes to Tokens vector and current.clear() makes current variable fresh to next use
        {
            if(!current.empty()){ // step 1: Yes Current is Empty  
                tokens.push_back(current); 
                current.clear();
            }
            
        }
        current+=c; // step 1: so C gets added to current fist step 2:it added chars until it finds Uppercase charecter 
    }
    if(!current.empty()){
        tokens.push_back(current);
    }
    
    

    for (const auto& token : tokens) {
        cout << token << endl;
    }
}