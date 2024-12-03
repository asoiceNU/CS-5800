#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

class VEBHeap {
private:
    int size;          // Current number of elements in the heap
    int min;           // Minimum value in the heap
    int max;           // Maximum value in the heap
    int sqrtM;         // Square root of the universe size
    std::vector<int> summary;        // The summary structure
    std::vector<std::vector<int>> clusters; // The cluster structure

    // Helper function to compute the high part of a value (index of the cluster)
    int high(int x) {
        return x / sqrtM;
    }

    // Helper function to compute the low part of a value (index within the cluster)
    int low(int x) {
        return x % sqrtM;
    }

    // Helper function to convert a cluster and value into a global index
    int index(int cluster, int x) {
        return cluster * sqrtM + x;
    }

    // Helper recursive function to insert a value into the appropriate cluster
    void insertRecursive(int x) {
        if (size == 0) {
            min = x;
            max = x;
            size = 1;
            return;
        }

        if (x < min) {
            std::swap(x, min);
        }

        if (x > max) {
            max = x;
        }

        int clusterIndex = high(x);
        int lowValue = low(x);

        if (clusters[clusterIndex].empty()) {
            // If the cluster is empty, initialize it with the low value
            clusters[clusterIndex].push_back(lowValue);
        } else {
            // Otherwise, insert recursively into the cluster
            insertRecursive(lowValue);
        }

        // Update the summary structure with the cluster index
        summary.push_back(clusterIndex);
        size++;
    }

    // Recursive function to delete the minimum element from the heap
    void deleteMinRecursive() {
        // If there's only one element in the heap, just reset it
        if (size == 1) {
            min = INT_MAX;
            max = INT_MIN;
            size = 0;
            return;
        }

        int clusterIndex = high(min);
        int lowValue = low(min);

        // Delete the min from the relevant cluster
        clusters[clusterIndex].clear();

        // Check if the cluster is now empty, and update the summary
        if (clusters[clusterIndex].empty()) {
            for (auto it = summary.begin(); it != summary.end(); ++it) {
                if (*it == clusterIndex) {
                    summary.erase(it);
                    break;
                }
            }
        }

        size--;

        // Recalculate the new minimum
        if (summary.empty()) {
            min = INT_MAX;
        } else {
            min = INT_MAX;
            for (int cluster : summary) {
                if (clusters[cluster].empty()) continue;
                min = std::min(min, clusters[cluster].front());
            }
        }
    }

    // Helper function for recursively decreasing the key
    void decreaseKeyRecursive(int oldKey, int newKey) {
        if (oldKey == newKey) {
            return;
        }

        if (newKey < min) {
            min = newKey;
        }

        int clusterIndex = high(oldKey);
        int lowValue = low(oldKey);

        // Remove the old value from the cluster
        clusters[clusterIndex].erase(std::remove(clusters[clusterIndex].begin(), clusters[clusterIndex].end(), lowValue), clusters[clusterIndex].end());

        // Insert the new value in the appropriate cluster
        if (clusters[clusterIndex].empty()) {
            // If the cluster becomes empty, remove it from the summary
            for (auto it = summary.begin(); it != summary.end(); ++it) {
                if (*it == clusterIndex) {
                    summary.erase(it);
                    break;
                }
            }
        }

        clusters[clusterIndex].push_back(newKey);

        // Update the minimum key if necessary
        if (newKey < min) {
            min = newKey;
        }
    }

public:
    VEBHeap(int universeSize) {
        size = 0;
        min = INT_MAX;
        max = INT_MIN;
        sqrtM = std::sqrt(universeSize);
        summary = std::vector<int>();
        clusters = std::vector<std::vector<int>>(sqrtM);
    }

    // Insert a value into the heap
    void insert(int x) {
        insertRecursive(x);
    }

    // Extract the minimum value from the heap
    int extractMin() {
        if (size == 0) {
            std::cerr << "Heap is empty!" << std::endl;
            return INT_MAX; // or throw an exception
        }
        return min;
    }

    // Delete the minimum value from the heap
    void deleteMin() {
        if (size == 0) {
            std::cerr << "Heap is empty!" << std::endl;
            return;
        }

        // Remove the minimum element from the heap
        deleteMinRecursive();

        // If the heap is not empty after deletion, recalculate the minimum
        if (size > 0) {
            // After deletion, we must find the new minimum
            min = INT_MAX;
            for (int clusterIndex : summary) {
                if (clusters[clusterIndex].empty()) continue;
                min = std::min(min, clusters[clusterIndex].front());
            }
        }
    }

    // Decrease the key of a given element
    void decreaseKey(int oldKey, int newKey) {
        if (newKey > oldKey) {
            std::cerr << "New key is greater than old key!" << std::endl;
            return; // Decrease key only works if the new key is smaller
        }

        decreaseKeyRecursive(oldKey, newKey);
    }

    // Print the current heap (for debugging)
    void printHeap() {
        std::cout << "Heap size: " << size << std::endl;
        std::cout << "Min: " << min << ", Max: " << max << std::endl;
        std::cout << "Summary: ";
        for (int index : summary) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
        std::cout << "Clusters: " << std::endl;
        for (int i = 0; i < clusters.size(); ++i) {
            std::cout << "Cluster " << i << ": ";
            for (int val : clusters[i]) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Test the VEB heap with decrease key operation
int main() {
    VEBHeap heap(16);

    heap.insert(5);
    heap.insert(3);
    heap.insert(8);
    heap.insert(12);

    std::cout << "Min value: " << heap.extractMin() << std::endl;
    
    heap.decreaseKey(8, 2);  // Decrease the key of 8 to 2
    std::cout << "Min value after decrease key: " << heap.extractMin() << std::endl;
    
    heap.printHeap();
    
    return 0;
}
