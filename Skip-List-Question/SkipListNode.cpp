#include "SkipListNode.hpp"

SkipListNode::SkipListNode(int value, int level) {
    this->value = value;
    forward.resize(level, nullptr);
}
