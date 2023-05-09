#include "graph_gen.h"

Graph GraphGenerator::generateGraph(int n, int m) {
    Graph graph(n, m);
    return graph;
}

std::vector<Graph> GraphGenerator::generateSaveGraphs(int n, int m, int numGraphs, std::string filename) {
    // Vector of graphs to save
    std::vector<Graph> graphs;

    // Reserve memory space for the graphs
    graphs.reserve(numGraphs);

    for (int i = 0; i < numGraphs; i++) {

        // Create a graph and add it to the vector
        graphs.emplace_back(n, m);
    }

    // Open the file to save the graphs to
    FILE *fp = fopen(filename.c_str(), "w");
    for (int i = 0; i < numGraphs; i++) {
        graphs[i].saveGraph(fp, i);
    }
    fclose(fp);
    return graphs;
}

void GraphGenerator::generateSaveGraph(int n, int m, std::string filename) {
    // Create a graph
    Graph graph(n, m);

    // Open the file to save the graph to
    FILE *fp = fopen(filename.c_str(), "w");
    graph.saveGraph(fp, 0);
    fclose(fp);
}

std::vector <Graph> GraphGenerator::loadGraphs(std::string filename) {
    // Vector of graphs to store the loaded graphs
    std::vector<Graph> graphs;

    // Open the file to load the graphs from
    FILE *fp = fopen(filename.c_str(), "r");

    // Read the graphs from the file
    int graphId, nbVertices, nbEdges;
    while (fscanf(fp, "Graph %d, Vertices %d, Edges %d\n", &graphId, &nbVertices, &nbEdges) != EOF) {
        graphs.emplace_back();
        std::vector<Edge> edges;
        edges.reserve(nbEdges);
        for (int i = 0; i < nbEdges; i++) {
            int fromVertex, toVertex;
            double edgeWeight;
            fscanf(fp, "%d %d %lf\n", &fromVertex, &toVertex, &edgeWeight);
            edges.emplace_back(fromVertex, toVertex, edgeWeight);
        }
        std::vector<int> vertices;
        vertices.reserve(nbVertices);
        for (int i = 0; i < nbVertices; i++) {
            vertices.emplace_back(i);
        }
        graphs[graphId] = Graph(nbVertices, nbEdges, edges, vertices);
    }
    return graphs;
}

int main() {
    GraphGenerator::generateSaveGraph(6, 20, "test.txt");
    return 0;
}