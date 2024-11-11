#include "SkipList.hpp"
#include <iostream>
#include <cstdlib>

SkipList::SkipList(int maxLevel, float probability) {
    this->maxLevel = maxLevel;
    this->probability = probability;
    this->level = 0;

    // Create a header node with a maximum level and no value
    header = new SkipListNode(-1, maxLevel);
}

int SkipList::randomLevel() {
    int lvl = 1;
    while (static_cast<float>(rand()) / RAND_MAX < probability && lvl < maxLevel) {
        lvl++;
    }
    return lvl;
}

SkipListNode* SkipList::search(int value) {
    SkipListNode* current = header;
    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];  // Move to the next node at level 0
    return (current && current->value == value) ? current : nullptr;
}

void SkipList::insert(int value) {
    std::vector<SkipListNode*> update(maxLevel, nullptr);
    SkipListNode* current = header;

    // Find the insertion point for each level
    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    // Move to the next node at level 0
    current = current->forward[0];

    // If the node already exists, don't insert it
    if (current && current->value == value) {
        return;
    }

    // Randomly determine the level of the new node
    int newLevel = randomLevel();

    // Update the level of the skip list if necessary
    if (newLevel > level) {
        for (int i = level; i < newLevel; i++) {
            update[i] = header;
        }
        level = newLevel;
    }

    // Create the new node
    SkipListNode* newNode = new SkipListNode(value, newLevel);

    // Update the forward pointers for the new node and the nodes in the update list
    for (int i = 0; i < newLevel; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

void SkipList::deleteNode(int value) {
    std::vector<SkipListNode*> update(maxLevel, nullptr);
    SkipListNode* current = header;

    // Find the node to delete
    for (int i = level - 1; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->value < value) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    // Move to the next node at level 0
    current = current->forward[0];

    // If the node is not found, return
    if (current == nullptr || current->value != value) {
        std::cout << "Node not found!" << std::endl;
        return;
    }

    // Update the forward pointers to remove the node
    for (int i = 0; i < level; i++) {
        if (update[i]->forward[i] != nullptr && update[i]->forward[i] == current) {
            update[i]->forward[i] = current->forward[i];
        }
    }

    // Decrease the level of the skip list if necessary
    while (level > 1 && header->forward[level - 1] == nullptr) {
        level--;
    }

    // Free the memory for the node
    delete current;
}

void SkipList::printList() {
    for (int i = 0; i < level; i++) {
        SkipListNode* current = header->forward[i];
        std::cout << "Level " << i << ": ";
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->forward[i];
        }
        std::cout << std::endl;
    }
}
