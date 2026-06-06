#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

int main(){
    unordered_map<string, int> fruits;
    fruits.insert({"apple", 3});
    fruits.insert({"banana", 5});
    fruits.insert({"cherry", 1});
    for(const auto &fruit:fruits){
        if(fruit.second >2){
            cout << fruit.first << " : " << fruit.second << endl;
        }
    }

}