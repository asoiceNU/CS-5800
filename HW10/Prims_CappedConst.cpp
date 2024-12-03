#include <iostream>
#include <vector>
#include <list>
#include <climits>

using namespace std;

// Define a structure for edges
struct Edge {
    int u, v, weight;
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
};

// A class to represent the priority queue using an array of doubly linked lists
class PrimPriorityQueue {
public:
    // An array of doubly linked lists for each possible edge weight
    vector<list<Edge>> weightBuckets; 
    vector<bool> inMST;  // To check if a node is included in MST
    
    PrimPriorityQueue(int n, int W) : inMST(n, false) {
        weightBuckets.resize(W + 1);  // The weight ranges from 1 to W, so W+1 buckets
    }

    // Add an edge to the priority queue (bucket for the given weight)
    void insert(Edge e) {
        weightBuckets[e.weight].push_back(e);
    }

    // Extract the minimum weight edge (EXTRACT-MIN operation)
    Edge extractMin() {
        // Find the first non-empty bucket (smallest weight with edges)
        for (int weight = 1; weight < weightBuckets.size(); weight++) {
            if (!weightBuckets[weight].empty()) {
                // Remove and return the first edge in the list
                Edge minEdge = weightBuckets[weight].front();
                weightBuckets[weight].pop_front();
                return minEdge;
            }
        }
        return Edge(-1, -1, INT_MAX);  // If no edge is available
    }

    // Decrease the key of a vertex (move it to a new bucket with a lower weight)
    void decreaseKey(Edge e, int newWeight) {
        // Remove the edge from the old bucket
        auto it = find(weightBuckets[e.weight].begin(), weightBuckets[e.weight].end(), e);
        if (it != weightBuckets[e.weight].end()) {
            weightBuckets[e.weight].erase(it);
        }
        
        // Insert the edge into the new weight bucket
        e.weight = newWeight;
        weightBuckets[newWeight].push_back(e);
    }

    // Mark a vertex as included in MST
    void markInMST(int vertex) {
        inMST[vertex] = true;
    }

    // Check if a vertex is already in MST
    bool isInMST(int vertex) {
        return inMST[vertex];
    }
};

// Function to run Prim's algorithm using the PrimPriorityQueue
void prim(const vector<vector<Edge>>& graph, int n, int W) {
    PrimPriorityQueue pq(n, W);
    vector<int> key(n, INT_MAX);  // Stores the minimum weight of edges for each node
    vector<int> parent(n, -1);    // Stores the parent of each node in the MST

    // Start with vertex 0, setting its key to 0 (it becomes the first node in MST)
    key[0] = 0;
    pq.insert(Edge(-1, 0, 0));  // We don't use the u part of the edge in this case, just a placeholder

    while (true) {
        // Extract the minimum key edge (find the next node to add to the MST)
        Edge minEdge = pq.extractMin();
        int u = minEdge.v;

        if (u == -1) break;  // If there are no more edges, stop

        // Mark node u as included in the MST
        pq.markInMST(u);

        // Explore all adjacent nodes to u
        for (const Edge& edge : graph[u]) {
            int v = edge.v;
            if (!pq.isInMST(v) && edge.weight < key[v]) {
                // Update the key and parent of node v, and insert the edge into the queue
                key[v] = edge.weight;
                parent[v] = u;
                pq.decreaseKey(edge, edge.weight);  // Update the weight of the edge in the priority queue
            }
        }
    }

    // Output the MST (or the edges in the MST)
    cout << "MST edges:" << endl;
    for (int i = 1; i < n; i++) {
        cout << parent[i] << " - " << i << endl;
    }
}

int main() {
    int n = 5;  // Number of nodes in the graph
    int W = 10;  // Maximum weight of an edge
    vector<vector<Edge>> graph(n);

    // Define some edges
    graph[0].push_back(Edge(0, 1, 2));
    graph[0].push_back(Edge(0, 3, 6));
    graph[1].push_back(Edge(1, 0, 2));
    graph[1].push_back(Edge(1, 2, 3));
    graph[1].push_back(Edge(1, 3, 8));
    graph[2].push_back(Edge(2, 1, 3));
    graph[2].push_back(Edge(2, 3, 5));
    graph[3].push_back(Edge(3, 0, 6));
    graph[3].push_back(Edge(3, 1, 8));
    graph[3].push_back(Edge(3, 2, 5));

    // Run Prim's algorithm
    prim(graph, n, W);

    return 0;
}
