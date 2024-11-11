#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>

using namespace std;

// A node in the skip list
struct Node {
    int value;
    vector<Node*> forward; // A vector to hold forward pointers for each level

    // Constructor for creating a new node
    Node(int value, int level) : value(value), forward(level, nullptr) {}
};

// Skip List class
class SkipList {
public:
    SkipList(float probability); // Constructor without maxLevel
    void insert(int value);
    void deleteNode(int value);
    bool lookup(int value);
    void printSkipList();

private:
    int randomLevel();
    Node* header; // Header node (sentinel)
    float probability; // Probability of promoting a node to a higher level
    int currentLevel; // Current highest level
};

// Skip List constructor
SkipList::SkipList(float probability) {
    this->probability = probability;
    currentLevel = 0;  // Initially, the list has only one level
    header = new Node(INT_MIN, 1); // Create the header node with value INT_MIN

    // Initialize the header's forward pointer to nullptr (it has only 1 level at start)
}

// Random level generator (probabilistic)
int SkipList::randomLevel() {
    int level = 0;
    while ((rand() % 100) < (probability * 100)) {
        level++;
    }
    return level;
}

// Insert a value into the skip list
void SkipList::insert(int value) {
    vector<Node*> update;  // To store the nodes where we need to update the forward pointer
    Node* current = header;

    // Find the position to insert by traversing levels from top to bottom
    for (int i = currentLevel - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update.push_back(current); // Record the last node at each level
    }

    current = current->forward[0]; // Move to the bottom level

    // If the value is not already in the skip list, insert it
    if (current == nullptr || current->value != value) {
        int level = randomLevel(); // Determine the level for the new node
        if (level > currentLevel) { // If level exceeds current highest level
            for (int i = currentLevel; i < level; i++) {
                update.push_back(header); // Update the header's forward pointers
            }
            currentLevel = level;
        }

        // Create a new node and insert it at appropriate levels
        current = new Node(value, level + 1);
        for (int i = 0; i <= level; i++) {
            current->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = current;
        }
    }
}

// Delete a node with a given value from the skip list
void SkipList::deleteNode(int value) {
    vector<Node*> update;
    Node* current = header;

    // Find the node to delete by traversing levels from top to bottom
    for (int i = currentLevel - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update.push_back(current); // Record the last node at each level
    }

    current = current->forward[0]; // Move to the bottom level

    if (current != nullptr && current->value == value) {
        // Remove the node from all levels
        for (int i = 0; i < currentLevel; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        // Decrease the level if necessary
        while (currentLevel > 0 && header->forward[currentLevel - 1] == nullptr) {
            currentLevel--;
        }

        delete current; // Free the memory
    }
}

// Lookup for a value in the skip list
bool SkipList::lookup(int value) {
    Node* current = header;

    // Traverse from the top level to bottom level
    for (int i = currentLevel - 1; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->value < value) {
            current = current->forward[i];
        }
    }

    current = current->forward[0]; // Move to the lowest level

    return current != nullptr && current->value == value;
}

// Print the entire skip list
void SkipList::printSkipList() {
    cout << "Skip List Levels:\n";
    for (int i = currentLevel - 1; i >= 0; i--) {
        Node* current = header->forward[i];
        cout << "Level " << i << ": ";
        while (current != nullptr) {
            cout << current->value << " ";
            current = current->forward[i];
        }
        cout << "\n";
    }
}

int main() {
    srand(time(0)); // Initialize random seed for randomLevel function

    SkipList list(0.5); // Probability of 50% for node promotion

    // Insert values
    list.insert(20);
    list.insert(40);
    list.insert(10);
    list.insert(20); // Duplicate insert (skip list should handle this)
    list.insert(5);
    list.insert(80);
    list.insert(100);
    list.insert(30);
    list.insert(50);

    // Print the skip list
    list.printSkipList();

    // Lookup values
    cout << "Lookup 80: " << (list.lookup(80) ? "Found" : "Not Found") << endl;
    cout << "Lookup 25: " << (list.lookup(25) ? "Found" : "Not Found") << endl;

    // Delete some values
    list.deleteNode(20);
    list.deleteNode(5);

    // Print the skip list again after deletions
    list.printSkipList();

    return 0;
}
