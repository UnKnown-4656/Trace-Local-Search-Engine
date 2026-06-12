#include "utils.h"


string input(string placeholder)
{
    string str;
    cout << placeholder;
    cin.ignore(); // Clear the input buffer
    getline(cin, str); //will read the whole line even if it contains spaces
    return str;
}