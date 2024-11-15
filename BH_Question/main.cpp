#include <iostream>
#include "BinomialHeap.hpp"

int main() {
    BinomialHeap bh;

    // Inserting elements into the binomial heap
    bh.insert(10);
    bh.insert(20);
    bh.insert(5);
    bh.insert(30);
    bh.insert(15);
    bh.insert(999);


    // Print the heap after insertions
    std::cout << "Heap after insertions: ";
    bh.print();

    // Get and print the minimum element in the heap
    std::cout << "Minimum element: " << bh.getMin() << std::endl;

    // Print the heap after finding min
    std::cout << "Heap after finding min (not extracting): ";
    bh.print();

    // Extracting the minimum element and printing the heap
    std::cout << "Extracting minimum: " << bh.extractMin() << std::endl;
    std::cout << "Heap after extracting minimum: ";
    bh.print();

    // Decrease a key and print the heap again
    std::cout << "Decreasing key 30 to 2" << std::endl;
    bh.decreaseKey(30, 2);
    std::cout << "Heap after decreasing key: ";
    bh.print();

    // Delete a key and print the heap again
    std::cout << "Deleting key 20" << std::endl;
    bh.deleteKey(20);
    std::cout << "Heap after deleting key: ";
    bh.print();

    // Insert more elements and print the heap
    bh.insert(8);
    bh.insert(25);
    bh.insert(7);

    std::cout << "Heap after more insertions: ";
    bh.print();

    //Union Demo--create a new Heap and then merge them:

    BinomialHeap bh2;
    bh2.insert(103);
    bh2.print();

    //merge bh2 --> bh
    bh.unionHeaps(bh2);

    //bh2 is now empty
    bh2.print();
    std::cout << "bh after merging bh2 into it: ";
    bh.print();

    std::cout << bh.extractMin() << " \n";
    bh.print();
    std::cout << bh.extractMin() << " \n";
    bh.print();

    bh.deleteKey(999);
    bh.print();

    bh.deleteKey(10);
    bh.print();

    

    // Extract all elements one by one
    std::cout << "Extracting elements: ";
    while (bh.getMin() != INT_MIN) {
        std::cout << bh.extractMin() << " ";
    }
    std::cout << std::endl;

    return 0;
}
