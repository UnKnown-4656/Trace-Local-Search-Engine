#include "Indexer/Indexer.h"
#include "SearchEngine/SearchEngine.h"
#include "Utils/utils.h"

int main() {
    fs::path MyPath = "D:\\Downloads";
    
    Indexer indexer;
    if (fs::exists("data/index.txt")) {
        indexer.LoadIndex("data/index.txt");
    } else {
        indexer.ScanFiles(MyPath);
        indexer.SaveIndex("data/index.txt");
    }

    SearchEngine engine(indexer.getFiles());

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
            indexer.ScanFiles(MyPath);
            indexer.SaveIndex("data/index.txt");
            cout << "Files Rescanned Successfully" << endl;
        }
        else if (user_choice == 3) {
            running = false;
        }
        else {
            cout << "Invalid Choice" << endl;
        }
    }
    return 0;
}