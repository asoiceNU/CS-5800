#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

// Binomial tree node structure
struct Node {
    int key;
    int degree;
    Node* parent;
    Node* child;
    Node* sibling;

    Node(int val) : key(val), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

// Binomial heap class
class BinomialHeap {
private:
    Node* head;  // Head of the linked list of binomial trees

    // Merges two binomial trees of the same degree
    Node* mergeTrees(Node* t1, Node* t2) {
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
    Node* mergeHeaps(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        Node *dummy = new Node(INT_MIN), *tail = dummy;
        Node *t1 = h1, *t2 = h2;

        while (t1 && t2) {
            if (t1->degree < t2->degree) {
                tail->sibling = t1;
                t1 = t1->sibling;
            } else if (t1->degree > t2->degree) {
                tail->sibling = t2;
                t2 = t2->sibling;
            } else {
                Node* merged = mergeTrees(t1, t2);
                tail->sibling = merged;
                t1 = t1->sibling;
                t2 = t2->sibling;
            }
            tail = tail->sibling;
        }

        // Append remaining trees
        if (t1) tail->sibling = t1;
        if (t2) tail->sibling = t2;

        Node* newHead = dummy->sibling;
        delete dummy;

        return newHead;
    }

    // Consolidates the heap to ensure there are no two trees of the same degree
    void consolidate() {
        if (!head) return;

        vector<Node*> degrees(32, nullptr);  // Binomial heap has at most log(n) trees, degree <= log(n)
        Node* prev = nullptr;
        Node* curr = head;
        Node* next = nullptr;

        while (curr) {
            next = curr->sibling;
            int d = curr->degree;

            // Handle trees with the same degree
            while (degrees[d]) {
                Node* other = degrees[d];
                if (other->key < curr->key) {
                    swap(curr, other);
                }

                if (curr->sibling == other) {
                    curr->sibling = other->sibling;
                }

                // Merge the two trees
                mergeTrees(curr, other);
                degrees[d] = nullptr;
                d++;
            }

            degrees[d] = curr;
            prev = curr;
            curr = next;
        }

        // Rebuild the heap from the degree array
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

    // Helper function to print binomial heap (in order)
    void printHeap(Node* root) {
        if (!root) return;

        Node* current = root;
        while (current) {
            cout << current->key << " ";
            printHeap(current->child);
            current = current->sibling;
        }
    }

    // Helper function to find a node by key (for Decrease Key and Delete)
    Node* findNode(Node* root, int key) {
        if (!root) return nullptr;
        if (root->key == key) return root;

        Node* res = findNode(root->child, key);
        if (res) return res;

        return findNode(root->sibling, key);
    }

public:
    BinomialHeap() : head(nullptr) {}

    // Insert a new key into the heap
    void insert(int key) {
        Node* newNode = new Node(key);
        head = mergeHeaps(head, newNode);
    }

    // Extract the minimum key from the heap
    int extractMin() {
        if (!head) return INT_MIN;

        Node* minNode = head;
        Node* minPrev = nullptr;
        Node* prev = nullptr;
        Node* curr = head;

        // Find the node with the minimum key
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

        // Reverse the order of the children of the min node
        Node* child = minNode->child;
        Node* reversedChild = nullptr;

        while (child) {
            Node* nextChild = child->sibling;
            child->sibling = reversedChild;
            child->parent = nullptr;
            reversedChild = child;
            child = nextChild;
        }

        // Merge the reversed child list with the rest of the heap
        head = mergeHeaps(head, reversedChild);
        
        int minValue = minNode->key;
        delete minNode;
        return minValue;
    }

    // Get the minimum key without extracting it
    int getMin() {
        if (!head) return INT_MIN;
        Node* curr = head;
        int minVal = INT_MAX;

        while (curr) {
            minVal = min(minVal, curr->key);
            curr = curr->sibling;
        }

        return minVal;
    }

    // Union of two binomial heaps
    void unionHeaps(BinomialHeap& other) {
        head = mergeHeaps(head, other.head);
        other.head = nullptr;
        consolidate();
    }

    // Decrease key operation
    void decreaseKey(int oldKey, int newKey) {
        if (newKey > oldKey) {
            cout << "New key is greater than the old key. Cannot decrease.\n";
            return;
        }

        // Find the node with oldKey
        Node* node = findNode(head, oldKey);
        if (!node) {
            cout << "Key not found!\n";
            return;
        }

        // Update the key
        node->key = newKey;

        // "Bubble-up" the node to restore the heap property
        Node* parent = node->parent;
        while (parent && node->key < parent->key) {
            swap(node->key, parent->key);
            node = parent;
            parent = parent->parent;
        }
    }

    // Delete a key from the heap
    void deleteKey(int key) {
        decreaseKey(key, INT_MIN);  // Decrease the key to the minimum possible value
        extractMin();  // Now extract the minimum (which is the deleted key)
    }

    // Print the heap
    void print() {
        printHeap(head);
        cout << endl;
    }
};

int main() {
    BinomialHeap bh;

    bh.insert(10);
    bh.insert(20);
    bh.insert(5);
    bh.insert(30);
    bh.insert(15);

    cout << "Heap after insertions: ";
    bh.print();

    cout << "Minimum element: " << bh.getMin() << endl;

    cout << "Extracting minimum: " << bh.extractMin() << endl;
    cout << "Heap after extracting minimum: ";
    bh.print();

    cout << "Extracting minimum: " << bh.extractMin() << endl;
    cout << "Heap after extracting minimum: ";
    bh.print();

    bh.decreaseKey(30, 2);
    cout << "Heap after decreasing key 30 to 2: ";
    bh.print();

    bh.deleteKey(2);
    cout << "Heap after deleting key 2: ";
    bh.print();

    BinomialHeap bh2;
    bh2.insert(100);
    bh2.insert(50);
    bh2.insert(40);

    cout << "Second heap: ";
    bh2.print();

    bh.unionHeaps(bh2);
   
}