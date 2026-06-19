#include "Indexer/sql_indexer.h"
#include "SearchEngine/SearchEngine.h"
#include "Utils/utils.h"


using namespace std;
namespace fs = std::filesystem;


int main() {
    fs::path MyPath = "D:\\Downloads";
    
    Indexer indexer;
    if (fs::exists("index.db")) {
        indexer.load_index("index.db");
    } else {
        //indexer.files.clear();
        indexer.ScanFiles(MyPath);
        indexer.save_index("index.db");
    }

    SearchEngine engine(&indexer.getFiles());

    bool running = true;
    while (running) {
        int user_choice;
        cout << "1. Search for a file" << endl;
        cout << "2. Rescan files" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> user_choice;

        if (user_choice == 1) {
            string Target = input("Enter Target File Name :");
            auto results =engine.search(Target);
            for(const auto &result :results){
                cout
                    << result.path
                    <<endl;
            }
        }
        else if (user_choice == 2) {
            //indexer.clear();
            indexer.ScanFiles(MyPath);
            indexer.save_index("index.db");
            engine = SearchEngine(&indexer.getFiles());
            cout << "Files Rescanned Successfully" << endl;
            
            
        }
        else if (user_choice == 3) {
            running = false;
        }
        else {
            cout << "Invalid Choice" << endl;
            break;
        }
    }
    return 0;
}