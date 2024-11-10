#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>

#define MAXHASH 300  // Change this value to 30, 300, or 1000 based on test case

using namespace std;

// Node structure for the linked list
struct Node {
    string word;
    int count;
    Node* next;

    Node(const string& w) : word(w), count(1), next(nullptr) {}
};

// Hash table structure
class HashTable {
public:
    HashTable(int size) : size(size) {
        table.resize(size, nullptr); // Initialize hash table with null pointers
    }

    // Simple hash function to hash words into a range of [0, size-1]
    unsigned int hash(const string& word) {
        unsigned int hash_value = 0;
        for (char ch : word) {
            hash_value = (hash_value * 31 + tolower(ch)) % size;
        }
        return hash_value;
    }

    // Insert a key-value pair into the hash table
    void insert(const string& word) {
        unsigned int index = hash(word);
        Node* current = table[index];

        // Check if the word already exists in the linked list
        while (current != nullptr) {
            if (current->word == word) {
                current->count++;  // Word found, increase count
                return;
            }
            current = current->next;
        }

        // Word not found, insert it at the beginning of the list
        Node* new_node = new Node(word);
        new_node->next = table[index];
        table[index] = new_node;
    }

    // Find a word and return its count, or -1 if not found
    int find(const string& word) {
        unsigned int index = hash(word);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->word == word) {
                return current->count;
            }
            current = current->next;
        }
        return -1;  // Word not found
    }

    // Delete a word from the hash table
    void deleteWord(const string& word) {
        unsigned int index = hash(word);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->word == word) {
                if (prev == nullptr) {
                    table[index] = current->next;  // Word is at the head
                } else {
                    prev->next = current->next;  // Remove the word from the list
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    // Increase the count of a word
    void increase(const string& word) {
        unsigned int index = hash(word);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->word == word) {
                current->count++;
                return;
            }
            current = current->next;
        }
        // If word is not found, insert it
        insert(word);
    }

    // Print all words and their counts
    void list_all_keys(ofstream& output_file) {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                output_file << current->word << ": " << current->count << endl;
                current = current->next;
            }
        }
    }

    // Count the number of collisions and list statistics
    void collision_statistics() {
        int total_collisions = 0;
        int max_length = 0;
        int collision_buckets = 0;
        vector<int> bucket_sizes(size, 0);

        for (int i = 0; i < size; i++) {
            int length = 0;
            Node* current = table[i];
            while (current != nullptr) {
                length++;
                current = current->next;
            }
            bucket_sizes[i] = length;
            if (length > 1) {
                total_collisions += (length - 1);
                collision_buckets++;
            }
            if (length > max_length) {
                max_length = length;
            }
        }

        // Print some basic statistics
        cout << "Total collisions: " << total_collisions << endl;
        cout << "Number of collision buckets: " << collision_buckets << endl;
        cout << "Max length of collision lists: " << max_length << endl;

        // Variance of list lengths
        double mean = 0;
        for (int i = 0; i < size; i++) {
            mean += bucket_sizes[i];
        }
        mean /= size;

        double variance = 0;
        for (int i = 0; i < size; i++) {
            variance += (bucket_sizes[i] - mean) * (bucket_sizes[i] - mean);
        }
        variance /= size;

        cout << "Variance of list lengths: " << variance << endl;
    }

private:
    int size;
    vector<Node*> table;  // Hash table with linked lists
};

// Clean up the hash table
void free_table(HashTable* ht) {
    delete ht;  // C++ will call the destructor automatically
}

int main() {
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

    return 0;
}
