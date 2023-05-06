#include <iostream>
#include <cmath>
#include <vector>
#include <random>
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
        // n = number of vertices
        // m = number of edges
        int n, m;
        std::vector<Edge> edges;
        std::vector<int> vertices;
        std::vector<int> degrees;
    public:
        Graph() {}
        Graph(int _n, int _m) : n(_n), m(_m) {
            edges.reserve(m);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            //generate graph randomly
            for (int i = 0; i < m; i++) {
                int u = rand() % n;
                int v = rand() % n;
                double w = dist(gen);
                addEdge(u, v, w);
            }
            void getDegrees();
            void setN(int _n);
            void setM(int _m);
            void setVertices(int _vertices);
        }
        void setEdges(std::vector<Edge> _edges) {
            edges = _edges;
        }
        void setVertices(int _vertices) {
            //arange from 0 to n-1 vertices
            for (int i = 0; i < _vertices; i++) {
                vertices.push_back(i);
            }
        }
        std::vector<Edge> getEdges() {
            return edges;
        }
        void addEdge(int u, int v, int w) {
            edges.emplace_back(u, v, w);
        }
        void setN(int _n) {
            n = _n;
        }
        void setM(int _m) {
            m = _m;
        }
        int getN() {
            return n;
        }
        int getM() {
            return m;
        }
        void printGraph() {
            for (int i = 0; i < m; i++) {
                std::cout << edges[i].getFrom() << " " << edges[i].getTo() << " " << edges[i].getWeight() << std::endl;
            }
        }
        void save(FILE *fp, int i) {
            fprintf(fp, "Graph %d, Vertices %d, Edges %d\n", i, n, m);
            for (int i = 0; i < m; i++) {
                fprintf(fp, "%d %d %d\n", edges[i].getFrom(), edges[i].getTo(), edges[i].getWeight());
            }
        }
        void load(FILE *fp) {
            fscanf(fp, "%d %d", &n, &m);
            for (int i = 0; i < m; i++) {
                int u, v, w;
                fscanf(fp, "%d %d %d", &u, &v, &w);
                addEdge(u, v, w);
            }
        }
        void computeDegrees() {
            degrees.resize(n);
            for (int i = 0; i < n; i++) {
                degrees[i] = 0;
            }
            for (int i = 0; i < m; i++) {
                degrees[edges[i].getFrom()]++;
            }
        }
        void setDegrees(std::vector<int> _degrees) {
            degrees = _degrees;
        }
        
};


std::vector<Graph> loadGraphs(std::string filename) {
    std::vector<Graph> graphs;
    FILE *fp = fopen(filename.c_str(), "r");
    // read from : 
    // Graph 0, Vertices 100, Edges 1000
    // 68 18 3
    // 1 81 266
    // 74 19 204
    while (!feof(fp)) {
        int graphId, n, m;
        fscanf(fp, "Graph %d, Vertices %d, Edges %d\n", &graphId, &n, &m);
        graphs.emplace_back();
        std::vector<Edge> edges;
        for (int i = 0; i < m; i++) {
            int u, v, w;
            fscanf(fp, "%d %d %d\n", &u, &v, &w);
            edges.emplace_back(u, v, w);
        }

        graphs[graphId].setVertices(n);
        graphs[graphId].setEdges(edges);
        graphs[graphId].setN(n);
        graphs[graphId].setM(m);
    }

        
    fclose(fp);
    return graphs;
}
