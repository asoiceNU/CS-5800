// C++ program for the above approach
#include <bits/stdc++.h>
using namespace std;

// Class for each node in the Binomial Heap
class Node {
public:
    int value;
    Node* parent;
    vector<Node*> children;
    int degree;
    bool marked;

    Node(int val) {
        value = val;
        parent = nullptr;
        children.clear();
        degree = 0;
        marked = false;
    }
};

// Class for the Binomial Heap data structure
class BinomialHeap {
public:
    vector<Node*> trees;
    Node* min_node;
    int count;

    // Constructor for the Binomial Heap
    BinomialHeap() {
        min_node = nullptr;
        count = 0;
        trees.clear();
    }

    // Check if the heap is empty
    bool is_empty() {
        return min_node == nullptr;
    }

    // Insert a new value into the heap
    void insert(int value) {
        Node* node = new Node(value);
        BinomialHeap heap;
        heap.trees.push_back(node);
        merge(heap);
    }

    // Get the minimum value in the heap
    int get_min() {
        return min_node->value;
    }

    // Extract the minimum value from the heap
    int extract_min() {
        Node* minNode = min_node;
        trees.erase(remove(trees.begin(), trees.end(), minNode), trees.end());
        BinomialHeap heap;
        heap.trees = minNode->children;
        merge(heap);
        _find_min();
        count -= 1;
        return minNode->value;
    }

    // Merge two binomial heaps
    void merge(BinomialHeap& other_heap) {
        trees.insert(trees.end(), other_heap.trees.begin(), other_heap.trees.end());
        count += other_heap.count;
        _find_min();
    }

    // Find the minimum value in the heap
    void _find_min() {
        min_node = nullptr;
        for (Node* tree : trees) {
            if (min_node == nullptr || tree->value < min_node->value) {
                min_node = tree;
            }
        }
    }

    // Decrease the key of a node
    void decrease_key(Node* node, int new_value) {
        if (new_value > node->value) {
            throw invalid_argument("New value is greater than the current value");
        }
        node->value = new_value;
        _bubble_up(node);
    }

    // Delete a specific node from the heap
    void delete_node(Node* node) {
        decrease_key(node, INT_MIN);
        extract_min();
    }

    // Perform the bubbling up operation
    void _bubble_up(Node* node) {
        Node* parent = node->parent;
        while (parent != nullptr && node->value < parent->value) {
            swap(node->value, parent->value);
            node = parent;
            parent = node->parent;
        }
    }

    // Link two trees together
    void _link(Node* tree1, Node* tree2) {
        if (tree1->value > tree2->value) {
            swap(tree1, tree2);
        }
        tree2->parent = tree1;
        tree1->children.push_back(tree2);
        tree1->degree += 1;
    }

    // Consolidate the trees in the heap
    void _consolidate() {
        int max_degree = static_cast<int>(floor(log2(count))) + 1;
        vector<Node*> degree_to_tree(max_degree + 1, nullptr);

        while (!trees.empty()) {
            Node* current = trees[0];
            trees.erase(trees.begin());
            int degree = current->degree;
            while (degree_to_tree[degree] != nullptr) {
                Node* other = degree_to_tree[degree];
                degree_to_tree[degree] = nullptr;
                if (current->value < other->value) {
                    _link(current, other);
                } else {
                    _link(other, current);
                    current = other;
                }
                degree++;
            }
            degree_to_tree[degree] = current;
        }

        min_node = nullptr;
        trees.clear();
        for (Node* tree : degree_to_tree) {
            if (tree != nullptr) {
                trees.push_back(tree);
                if (min_node == nullptr || tree->value < min_node->value) {
                    min_node = tree;
                }
            }
        }
    }

    // Get the size of the heap
    int size() {
        return count;
    }
};

