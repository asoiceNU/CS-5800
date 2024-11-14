#ifndef BINOMIALHEAP_HPP
#define BINOMIALHEAP_HPP

#include "BH_Node.hpp"

class BinomialHeap {
private:
    BH_Node* head;  // Head of the linked list of binomial trees

    BH_Node* mergeTrees(BH_Node* t1, BH_Node* t2);
    BH_Node* mergeHeaps(BH_Node* h1, BH_Node* h2);
    void consolidate();
    void printHeap(BH_Node* root);
    BH_Node* findNode(BH_Node* root, int key);

public:
    BinomialHeap();
    
    void insert(int key);
    int extractMin();
    int getMin();
    void unionHeaps(BinomialHeap& other);
    void decreaseKey(int oldKey, int newKey);
    void deleteKey(int key);
    void print();
};

#endif // BINOMIALHEAP_HPP
