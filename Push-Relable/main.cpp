#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

class PushRelabel {
public:
    PushRelabel(int n) {
        V = n;  // Number of vertices
        capacity.assign(n, vector<int>(n, 0));
        flow.assign(n, vector<int>(n, 0));
        height.assign(n, 0);
        excess.assign(n, 0);
    }

    void addEdge(int u, int v, int cap) {
        capacity[u][v] += cap;  // Add capacity for the edge u -> v
    }

    int getMaxFlow(int source, int sink) {
        height[source] = V;
        excess[source] = INT_MAX;

        // Initialize the preflow: send as much flow as possible from source to its neighbors
        for (int v = 0; v < V; ++v) {
            if (capacity[source][v] > 0) {
                flow[source][v] = capacity[source][v];
                flow[v][source] = -flow[source][v];
                excess[v] = flow[source][v];
                excess[source] -= flow[source][v];
            }
        }

        // Main loop: push and relabel until there are no excess flows
        while (true) {
            int vertex = -1;
            for (int v = 0; v < V; ++v) {
                if (v != source && v != sink && excess[v] > 0) {
                    vertex = v;
                    break;
                }
            }

            if (vertex == -1) break;  // No excess flow left

            // Attempt to push or relabel the vertex with excess flow
            push(vertex);
            if (excess[vertex] > 0) {
                relabel(vertex);
            }
        }

        // The maximum flow is the total excess flow at the sink
        return excess[sink];
    }

private:
    int V;  // Number of vertices
    vector<vector<int>> capacity;  // Capacity matrix
    vector<vector<int>> flow;  // Flow matrix
    vector<int> height;  // Height of each vertex
    vector<int> excess;  // Excess flow at each vertex

    // Push excess flow from vertex to a neighboring vertex
    void push(int vertex) {
        for (int v = 0; v < V; ++v) {
            if (capacity[vertex][v] - flow[vertex][v] > 0 && height[vertex] == height[v] + 1) {
                int pushFlow = min(excess[vertex], capacity[vertex][v] - flow[vertex][v]);
                flow[vertex][v] += pushFlow;
                flow[v][vertex] -= pushFlow;
                excess[vertex] -= pushFlow;
                excess[v] += pushFlow;

                if (excess[vertex] == 0) {
                    break;  // If no excess flow is left, stop pushing
                }
            }
        }
    }

    // Relabel the vertex to increase its height
    void relabel(int vertex) {
        int minHeight = INT_MAX;
        for (int v = 0; v < V; ++v) {
            if (capacity[vertex][v] - flow[vertex][v] > 0) {
                minHeight = min(minHeight, height[v]);
            }
        }
        height[vertex] = minHeight + 1;
    }
};

int main() {
    int n, m;
    cout << "Enter number of vertices and edges: ";
    cin >> n >> m;

    PushRelabel pr(n);

    cout << "Enter edges (u, v, capacity): \n";
    for (int i = 0; i < m; ++i) {
        int u, v, cap;
        cin >> u >> v >> cap;
        pr.addEdge(u, v, cap);
    }

    int source, sink;
    cout << "Enter source and sink vertices: ";
    cin >> source >> sink;

    int maxFlow = pr.getMaxFlow(source, sink);
    cout << "Maximum Flow: " << maxFlow << endl;

    return 0;
}
