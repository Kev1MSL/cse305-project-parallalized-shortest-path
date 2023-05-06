#include <iostream>
#include "graph_gen.cpp"
#include <iostream>

// Dijkstra's algorithm sequentially
void dijkstra(Graph g, int s, int t) {
    int n = g.getN();
    int m = g.getM();
    std::vector<Edge> edges = g.getEdges();
    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);
    dist[s] = 0;
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (dist[u] == INF) {
            break;
        }
        visited[u] = true;
        for (int j = 0; j < m; j++) {
            int v = edges[j].getTo();
            int w = edges[j].getWeight();
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }
    if (dist[t] == INF) {
        std::cout << "No path found" << std::endl;
    } else {
        std::vector<int> path;
        for (int v = t; v != -1; v = prev[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());
        std::cout << "Path: ";
        for (int i = 0; i < path.size(); i++) {
            std::cout << path[i] << " ";
        }
        std::cout << std::endl;

    }
}

int main(){
    // Load the graphs:
    std::vector<Graph> graphs = loadGraphs("test_100v_1000e_100g.txt");

    // Run Dijkstra's algorithm on each graph:
    for (int i = 0; i < graphs.size(); i++) {
        std::cout << "Graph " << i << std::endl;
        dijkstra(graphs[i], 0, 99);
    }

    return 0;
}