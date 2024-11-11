
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

#include "hash_table.hpp"

using namespace std;

int main() {
    //cout << "0\n";
    HashTable ht(MAXHASH);
    ifstream input_file("alice_in_wonderland.txt");
    ofstream output_file("word_counts.txt");

    if (!input_file.is_open()) {
        cerr << "Could not open input file." << endl;
        return 1;
    }

    string word;
    while (input_file >> word) {
        // Convert to lowercase and remove non-alphabetic characters
        word.erase(remove_if(word.begin(), word.end(), [](char c) {
            return !isalpha(c);
        }), word.end());

        // Insert the cleaned word into the hash table
        if (!word.empty()) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            ht.insert(word);
        }
    }

    

    // Output the list of words and counts
    ht.list_all_keys(output_file);

    // Print collision statistics
    ht.collision_statistics();

    input_file.close();
    output_file.close();

    ht.insert("awooooga");

    cout << ht.find("alice") <<"\n";
    ht.deleteWord("alice");
    cout << "After deletion: " << ht.find("alice") << "\n";
    cout << "'find' returns -1 if the key is NIL\n";

    cout << ht.find("the");
    cout << "test";
    

    return 0;
}