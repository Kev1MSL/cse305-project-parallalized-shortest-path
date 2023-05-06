#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ctime>

#define INF 0x3f3f3f3f

// struct representing an edge in the graph
class Edge {
    private:
        int u, v, w;
    public:
        
        Edge(int _u, int _v, int _w) : u(_u), v(_v), w(_w) {}

        int getFrom() {
            return u;
        }
        int getTo() {
            return v;
        }
        int getWeight() {
            return w;
        }
};
class Graph {
    private: 
        int n, m;
        std::vector<Edge> edges;
    public:
        
        Graph(int _n, int _m) : n(_n), m(_m) {
            edges.reserve(m);
            //generate graph randomly
            for (int i = 0; i < m; i++) {
                int u = rand() % n;
                int v = rand() % n;
                int w = rand() % 1000;
                addEdge(u, v, w);
            }
        }
        void setEdges(std::vector<Edge> _edges) {
            edges = _edges;
        }
        std::vector<Edge> getEdges() {
            return edges;
        }
        void addEdge(int u, int v, int w) {
            edges.emplace_back(u, v, w);
        }
        int getN() {
            return n;
        }
        int getM() {
            return m;
        }
};

// comparator function for sorting edges by weight
bool cmp(Edge e1, Edge e2) {
    return e1.w < e2.w;
}


