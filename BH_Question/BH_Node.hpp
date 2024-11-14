#ifndef BH_NODE_HPP
#define BH_NODE_HPP

class BH_Node {
public:
    int key;
    int degree;
    BH_Node* parent;
    BH_Node* child;
    BH_Node* sibling;

    BH_Node(int val);
};

#endif // BH_NODE_HPP
