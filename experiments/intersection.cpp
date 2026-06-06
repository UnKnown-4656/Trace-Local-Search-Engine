#include "header.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;



int main(){
    unordered_map<string, unordered_set<string>> file_index = {

    {"apple", {
        "D\\apple_pie.txt",
        "D\\apple2.txt"
    }},

    {"pie", {
        "D\\apple.txt",
        "D\\apple_pie.txt"
    }}
    };

    //unordered_set<string> set1 = {"apple", "banna","cherry"};
    //unordered_set<string> set2 = {"banana", "cherry", "date"};
    //for(const auto &item : ){
     //   if(set2.find(item)!=set2.end()){
       //     cout << item << endl;
      //  }
    //}
    //for(const auto &entry :file_index){
        //for(const auto &item : entry.second){
        //}
    //}
    cout << "Enter The Target :" <<endl;
    string target;
    cin >> target;
    vector<string> tokenized_target = Tokenize(target);
    unordered_set<string> result_set =file_index[tokenized_target[0]]; // initialize result_set with the first token's file paths
    for(const auto &token : tokenized_target){ // if input is "apple pie" then tokenized_target will be ["apple", "pie"]
        unordered_set<string> current_set = file_index[token]; // get the file paths for the current token
        unordered_set<string> temp_set;
        for(const auto &item : result_set){
            if(current_set.find(item)!=current_set.end()){ // check if the item in result_set is also in current_set
                temp_set.insert(item); // if it is, add it to temp_set
            }
            
        }
        result_set = temp_set; // update result_set to be the intersection of result_set and current_set

    }
    if(result_set.empty()){
        cout << "File Not Found" << endl;
    }
    else{
        for(const auto &item : result_set){
            cout << item << endl;
        }
    }

    return 0;
}