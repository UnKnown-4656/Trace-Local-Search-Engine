#include "Indexer/sql_indexer.h"
#include "SearchEngine/SearchEngine.h"
#include "Utils/utils.h"


using namespace std;
namespace fs = std::filesystem;

int main()
{
    fs::path PATH="C:\\Users\\super\\OneDrive\\Desktop\\shortcuts\\Trace-Local-Search-Engine\\tests\\Samples";
    Indexer indexer;
    indexer.ScanFiles(PATH);
    indexer.save_index("test.db");

    SearchEngine engine(&indexer.getFiles());
    unordered_map<string ,unordered_set<string>>files=
    {
        {"python-3.14.8",{(PATH /"python-3.14.8.txt").string()}},
        {"python",{(PATH /"python-3.14.8.txt").string()}},
        {"3.14.8",{(PATH /"python-3.14.8.txt").string()}},
        {"python-3.14.8-amd64",{(PATH /"python-3.14.8-amd64.txt").string()}}
    };

    //string file_names[]={"python-3.14.8","winrar","test"};
    for(const auto &file:files)
    {
        //cout <<"Results for "<<file.first<<" is :"<<endl;
        auto results=engine.search(file.first);
        for(auto &path:file.second)
        {
            for(auto result:results){
                if(result.path==path){
                    cout<<file.first<<" :PASS[MATCHED]"<<endl;
                }

            }
           // if(results[0].path==path){
             //   cout<<file.first<<" :PASS[MATCHED]"<<endl;
           // }
            //else{
            //    cout<<file.first<<" :FAIL[NOT MATCHED]"<<endl;
             ////   cout<<path<<endl;
             //   cout<<results[0].path<<endl;
            //}
        }
        //for(auto &result:results)
        ////{
        //    cout<<result.path<<" "<<result.score<<endl;
        //}
    }
    

    
}