#include "Indexer.h"

vector<string> Indexer::Tokenize(string &str) {
    vector<string> tokens;
    string current;
    for (char c :str){
        c=tolower(c);
        if(c=='_' || c=='.' || c=='-' || c==' ' || c=='(' || c==')' || c== '[ ' || c==']'){
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
void Indexer::ScanFiles(fs::path Path){

        try
    {
        for (const auto &entry : fs::directory_iterator(Path))
        {
            if (entry.is_directory())
            {
                ScanFiles(entry.path());
            }
            if (entry.is_regular_file())
            {
                string filename = entry.path().filename().string();
                //string LowerFilename = ToLower(filename);
                string path = entry.path().string();
                vector<string> tokenized_filename = Tokenize(filename);
            // cout <<entry.path()<<endl;
                for(const auto &token : tokenized_filename){
                    files[token].insert(path);
                }
                //files.insert({ToLower(entry.path().filename().string()), entry.path().string()});
            }
        }
    }
    catch (exception &e)
    {
        cout << "Error : " << e.what() << endl;
        return;
    }

}
void Indexer::SaveIndex(string filename){
    ofstream Index(filename);
    if (!Index.is_open())
    {
        cout << "Error : Could not open file" << endl;
        return;
    }
    else{
        for (const auto &entry : files)
        {
            //Index << entry.first << "|" << entry.second << endl;
            for(const auto &path : entry.second){
                Index << entry.first << "|" << path << endl;
            }
        }
        cout << "Index Saved Successfully" << endl;
    }
}
void Indexer::LoadIndex(string filename){
    ifstream Index(filename);

    if (!Index.is_open())
    {
        cout << "Error : Could not open file" << endl;
        return;
    }
    else
    {
        string line;
        while (getline(Index, line))
        {
            size_t delimiter = line.find("|");
            if (delimiter != string::npos)
            {
                string key = line.substr(0, delimiter);
                //vector<string> tokenized_key = Tokenize(key);
                string value = line.substr(delimiter + 1);
                //for(const auto &token : tokenized_key){
                files[key].insert(value);
                //}
                //files.insert({key, value});
            }
        }
        cout << "Index Loaded Successfully" << endl;
    }
}
const unordered_map<string, unordered_set<string>>& Indexer::getFiles() const {
    return files;
}