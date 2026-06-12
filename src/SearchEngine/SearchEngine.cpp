#include "SearchEngine.h"

using namespace std;

SearchEngine::SearchEngine(const unordered_map<string, unordered_set<string>>& files)
    : files(files)
{
}
vector<string>SearchEngine::Tokenize(string &str){
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
    
vector<SearchResult> SearchEngine::search(string Target)
    {
    bool found = false;
    //string LowerTarget = ToLower(Target);
    //string file_type = fs::path(LowerTarget).extension().string();
    vector <string> tokenized_target = Tokenize(Target);
    unordered_map<string,int> scores;
    vector<SearchResult> results;

    try
    {
        //Intersection(files,Target);
        for (const auto &token : tokenized_target)
        {
            //auto range = files.equal_range(token);
            auto it = files.find(token);
            if(it!=files.end()){
                for(const auto &path : it->second){
                   // cout << path << endl;
                    //found = true;
                    scores[path]++;
                }
            }
        }
        if (scores.empty())
        {
            cout << "File Not Found" << endl;
            return {};
        }
        
        for(const auto &item:scores){
            results.push_back(
                {
                    item.first,
                    item.second

                });
            }
        sort(
            results.begin(),
            results.end(),
            [](const SearchResult&a,const SearchResult&b)
            {
                return a.score > b.score;
            }
            );
        return results;
            

    }

    catch (exception &e)
    {
        cout << "Error : " << e.what() << endl;
        return{};
    }
    }       

void SearchEngine::searchwithintersection(string target){
   try{
     vector<string>tokenized_target=Tokenize(target);
     unordered_set<string>result_set=files.at(tokenized_target[0]); //First Token 
     for(const auto &token:tokenized_target){
         unordered_set<string>current_set=files.at(token);
         unordered_set<string>temp_set;
         for(const auto &item:result_set){
             if(current_set.find(item)!=current_set.end()){
                 temp_set.insert(item);
             } 

         }
         result_set=temp_set;

     }
     if(result_set.empty()){
        cout <<"File Not Found...."<<endl;

     }
     else{
         for(const auto &item:result_set){
             cout << item<<endl;
         }
     }
    }
    catch(exception &e){
        cout << "Files Not Found...." <<endl;
        return ;
    }
}