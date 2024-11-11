#ifndef SKIPLIST_HPP
#define SKIPLIST_HPP

#include "SkipListNode.hpp"
#include <vector>

class SkipList {
private:
    int maxLevel;
    float probability;
    SkipListNode* header;
    int level;

public:
    SkipList(int maxLevel = 16, float probability = 0.5);

    int randomLevel();
    SkipListNode* search(int value);
    void insert(int value);
    void deleteNode(int value);
    void printList();
};

#endif // SKIPLIST_HPP
