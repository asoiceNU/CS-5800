#include "BinomialHeap.hpp"
#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

// Merges two binomial trees of the same degree
BH_Node* BinomialHeap::mergeTrees(BH_Node* t1, BH_Node* t2) {
    if (t1->key > t2->key) {
        swap(t1, t2);
    }

    t2->parent = t1;
    t2->sibling = t1->child;
    t1->child = t2;
    t1->degree++;

    return t1;
}

// Merges two binomial heaps
BH_Node* BinomialHeap::mergeHeaps(BH_Node* h1, BH_Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    BH_Node *dummy = new BH_Node(INT_MIN), *tail = dummy;
    BH_Node *t1 = h1, *t2 = h2;

    while (t1 && t2) {
        if (t1->degree < t2->degree) {
            tail->sibling = t1;
            t1 = t1->sibling;
        } else if (t1->degree > t2->degree) {
            tail->sibling = t2;
            t2 = t2->sibling;
        } else {
            BH_Node* merged = mergeTrees(t1, t2);
            tail->sibling = merged;
            t1 = t1->sibling;
            t2 = t2->sibling;
        }
        tail = tail->sibling;
    }

    // Append remaining trees
    if (t1) tail->sibling = t1;
    if (t2) tail->sibling = t2;

    BH_Node* newHead = dummy->sibling;
    delete dummy;

    return newHead;
}

// Consolidates the heap to ensure there are no two trees of the same degree
void BinomialHeap::consolidate() {
    if (!head) return;

    vector<BH_Node*> degrees(32, nullptr);
    BH_Node* prev = nullptr;
    BH_Node* curr = head;
    BH_Node* next = nullptr;

    while (curr) {
        next = curr->sibling;
        int d = curr->degree;

        while (degrees[d]) {
            BH_Node* other = degrees[d];
            if (other->key < curr->key) {
                swap(curr, other);
            }

            if (curr->sibling == other) {
                curr->sibling = other->sibling;
            }

            mergeTrees(curr, other);
            degrees[d] = nullptr;
            d++;
        }

        degrees[d] = curr;
        prev = curr;
        curr = next;
    }

    head = nullptr;
    for (int i = 0; i < 32; i++) {
        if (degrees[i]) {
            if (!head) {
                head = degrees[i];
            } else {
                head = mergeHeaps(head, degrees[i]);
            }
        }
    }
}

// Insert a new key into the heap
void BinomialHeap::insert(int key) {
    // Create a new node
    BH_Node* newNode = new BH_Node(key);
    
    // Merge the new node with the existing heap
    head = mergeHeaps(head, newNode);
}

// Extract the minimum key from the heap
int BinomialHeap::extractMin() {
    if (!head) return INT_MIN;

    BH_Node* minNode = head;
    BH_Node* minPrev = nullptr;
    BH_Node* prev = nullptr;
    BH_Node* curr = head;

    while (curr) {
        if (curr->key < minNode->key) {
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    if (minPrev) minPrev->sibling = minNode->sibling;
    else head = minNode->sibling;

    BH_Node* child = minNode->child;
    BH_Node* reversedChild = nullptr;

    while (child) {
        BH_Node* nextChild = child->sibling;
        child->sibling = reversedChild;
        child->parent = nullptr;
        reversedChild = child;
        child = nextChild;
    }

    head = mergeHeaps(head, reversedChild);
    
    int minValue = minNode->key;
    delete minNode;
    return minValue;
}

// Get the minimum key without removing it
int BinomialHeap::getMin() {
    if (!head) return INT_MIN;
    BH_Node* curr = head;
    int minVal = INT_MAX;

    while (curr) {
        minVal = min(minVal, curr->key);
        curr = curr->sibling;
    }

    return minVal;
}

// Union of two binomial heaps
void BinomialHeap::unionHeaps(BinomialHeap& other) {
    head = mergeHeaps(head, other.head);
    other.head = nullptr;
    consolidate();
}

// Decrease the key of a node
void BinomialHeap::decreaseKey(int oldKey, int newKey) {
    if (newKey > oldKey) {
        std::cout << "New key is greater than the old key. Cannot decrease.\n";
        return;
    }

    BH_Node* node = findNode(head, oldKey);
    if (!node) {
        std::cout << "Key not found!\n";
        return;
    }

    // Decrease the key
    node->key = newKey;

    // Percolate up the tree to restore heap property
    BH_Node* parent = node->parent;
    while (parent && node->key < parent->key) {
        std::swap(node->key, parent->key);
        node = parent;
        parent = parent->parent;
    }
}

// Delete a key from the heap
void BinomialHeap::deleteKey(int key) {
    decreaseKey(key, INT_MIN);  // Decrease the key to the smallest possible value
    extractMin();  // Now extract the minimum (which is the deleted key)
}

// Helper function to print binomial heap (in order)
void BinomialHeap::printHeap(BH_Node* root) {
    if (!root) return;

    BH_Node* current = root;

    while (current) {
        std::cout << current->key << " ";
        printHeap(current->child);
        current = current->sibling;
    }
}

// Helper function to find a node by key (for Decrease Key and Delete)
BH_Node* BinomialHeap::findNode(BH_Node* root, int key) {
    if (!root) return nullptr;
    if (root->key == key) return root;

    BH_Node* res = findNode(root->child, key);
    if (res) return res;

    return findNode(root->sibling, key);
}

BinomialHeap::BinomialHeap() : head(nullptr) {}

void BinomialHeap::print() {
    printHeap(head);
    std::cout << std::endl;
}
