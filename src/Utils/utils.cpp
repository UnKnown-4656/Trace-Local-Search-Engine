#include "utils.h"

string ToLower(string str){
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}
string input(string placeholder)
{
    string str;
    cout << placeholder;
    cin.ignore(); // Clear the input buffer
    getline(cin, str); //will read the whole line even if it contains spaces
    return str;
}