#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

#define MAXHASH 300  // Change this value to 30, 300, or 1000 based on test case

struct Node {
    string word;
    int count;
    Node* next;

    Node(const string& w) : word(w), count(1), next(nullptr) {}
};

class HashTable{
    public:
        HashTable(int size);
        unsigned int hash(const string& word);
        void insert(const string& word);

        int find(const string& word);
        void deleteWord(const string& word);
        void increase(const string& word);
        void list_all_keys(ofstream& output_file);
        void collision_statistics();
    private:
        int size;
        vector<Node*> table;
        void print_histogram(const vector<int>& bucket_sizes);
};

void free_table(HashTable* ht);

#endif