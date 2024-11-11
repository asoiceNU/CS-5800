#include <iostream>
#include "SkipList.hpp"

int main() {
    srand(time(0)); // Seed for random level generation

    SkipList skipList;

    // Insert elements
    skipList.insert(3);
    skipList.insert(6);
    skipList.insert(7);
    skipList.insert(9);
    skipList.insert(12);
    skipList.insert(19);
    skipList.insert(17);

    std::cout << "Skip List after inserts:" << std::endl;
    skipList.printList();

    // Search for elements
    SkipListNode* result = skipList.search(6);
    if (result) {
        std::cout << "Found 6 in the skip list." << std::endl;
    } else {
        std::cout << "6 not found." << std::endl;
    }

    // Delete a node
    skipList.deleteNode(6);
    std::cout << "Skip List after deleting 6:" << std::endl;
    skipList.printList();

    return 0;
}
