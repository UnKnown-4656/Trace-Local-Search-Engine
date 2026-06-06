#include <iostream>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    string filepath;
    TrieNode() {
        isEndOfWord = false;
        filepath = "";
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};
void insert(TrieNode* root, const string& word, const string& filepath) {
    TrieNode* currentNode = root;
    for (char c : word) {
        int index = c - 'a';
        if (currentNode->children[index] == nullptr) {
            currentNode->children[index] = new TrieNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->isEndOfWord = true;
    currentNode->filepath = filepath;
}
void search(TrieNode* root, const string& word){
    TrieNode* currentNode = root; // Start from the root node
    for (char c : word){ // Traverse the trie based on the characters of the word
        int index = c - 'a'; // Calculate the index for the current character
        if(currentNode->children[index]==nullptr){ // If the child node for the current character does not exist, the word is not found
            cout << "Word not found" << endl;

            return;
        }
        currentNode=currentNode->children[index]; // Move to the child node for the current character

    }
    if (currentNode->isEndOfWord) { // If we have reached the end of the word, it means the word is found
        cout << "Word found with filepath: " << currentNode->filepath << endl; // Print the associated filepath
    } else {
        cout << "Word not found" << endl; // If we have reached the end of the traversal but the isEndOfWord flag is not set, it means the word is not found
    }
}
void collectAllWords(TrieNode* node, string currentWord) {
    if (node->isEndOfWord) { // If the current node marks the end of a word, print the word and its associated filepath
        cout << "Word: " << currentWord << ", Filepath: " << node->filepath << endl;
    }
    for (int i = 0; i < 26; i++) { // Traverse all children of the current node
        if (node->children[i] != nullptr) { // If the child node exists, continue collecting words from that child
            collectAllWords(node->children[i], currentWord + char(i + 'a')); // Append the character corresponding to the child index to the current word and continue traversal
        }
    }
}
int main() {
    TrieNode* root = new TrieNode();
    insert(root, "hello", "/path/to/hello");
    search(root, "hello");
    collectAllWords(root, "");
    
    return 0;
}