#ifndef SKIPLISTNODE_HPP
#define SKIPLISTNODE_HPP

#include <vector>

class SkipListNode {
public:
    int value;
    std::vector<SkipListNode*> forward;

    SkipListNode(int value, int level);
};

#endif // SKIPLISTNODE_HPP
