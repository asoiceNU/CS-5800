#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

#include "hash_table.hpp"

extern int __Z10free_tableP9HashTable;

using namespace std;

// Node structure for the linked list

// Hash table structure
//class HashTable {
//public:
    HashTable::HashTable(int size) : size(size) {
        table.resize(size, nullptr); // Initialize hash table with null pointers
    }

    // Simple hash function to hash words into a range of [0, size-1]
    unsigned int HashTable::hash(const string& word) {
        unsigned int hash_value = 0;
        for (char ch : word) {
            hash_value = (hash_value * 31 + tolower(ch)) % size;
        }
        return hash_value;
    }

    // Insert a key-value pair into the hash table
    void HashTable::insert(const string& word) {
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
    int HashTable::find(const string& word) {
        unsigned int index = hash(word);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->word == word) {
                cout << "Key count: " << current -> count << endl;
                return current->count;
            }
            current = current->next;
        }
        return -1;  // Word not found
        cout << "Key Not Found";
        
    }

    // Delete a word from the hash table
    void HashTable::deleteWord(const string& word) {
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
    void HashTable::increase(const string& word) {
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
    void HashTable::list_all_keys(ofstream& output_file) {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                output_file << current->word << ": " << current->count << endl;
                current = current->next;
            }
        }
    }

    // Count the number of collisions and list statistics
    void HashTable::collision_statistics() {
        vector<int> bucket_sizes(size, 0);
        int total_collisions = 0;
        int max_length = 0;
        int collision_buckets = 0;

        // Collect collision list sizes
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

        // Print basic statistics
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

        // Generate and print histogram of collision list lengths
        print_histogram(bucket_sizes);
    }

//private:
//    int size;
//    vector<Node*> table;  // Hash table with linked lists

    // Function to print the histogram of collision list lengths
    void HashTable::print_histogram(const vector<int>& bucket_sizes) {
        // Create a map to count frequency of each collision list length
        vector<int> histogram;
        for (int size : bucket_sizes) {
            if (size > 0) {
                if (size >= histogram.size()) {
                    histogram.resize(size + 1, 0);
                }
                histogram[size]++;
            }
        }

        // Print the histogram
        cout << "\nHistogram of Collision List Lengths:\n";
        for (int i = 1; i < histogram.size(); i++) {
            if (histogram[i] > 0) {
                cout << "Length " << i << ": " << histogram[i] << " buckets\n";
            }
        }
    }
//};

// Clean up the hash table
void free_table(HashTable* ht) {
    delete ht;  // C++ will call the destructor automatically
}

