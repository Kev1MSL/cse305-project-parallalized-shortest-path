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

    while (feof(fp) == 0) {
        int graphId, nbVertices, nbEdges;
        fscanf(fp, "Graph %d, Vertices %d, Edges %d\n", &graphId, &nbEdges, &nbVertices);
        graphs.emplace_back();
        std::vector<Edge> edges;
        edges.reserve(nbEdges);
        for (int i = 0; i < nbEdges; i++) {
            int fromVertex, toVertex, edgeWeight;
            fscanf(fp, "%d %d %d\n", &fromVertex, &toVertex, &edgeWeight);
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

/**
 * Generate a graph with n vertices and m edges, and save it to a file.
 * argv[1] = number of vertices
 * argv[2] = number of edges
 * argv[3] = number of graphs
 * argv[4] = filename
*/
int main(int argc, char* argv[]) {
    srand(time(NULL));
    // Generate and save graphs
    if (argc != 5) {
        fprintf(stderr, "[Error] Usage: ./graph_gen <number of vertices> <number of edges> <number of graphs> <filename>\n");
        return -1;
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int numGraphs = atoi(argv[3]);
    std::string filename = argv[4];
    GraphGenerator::generateSaveGraphs(5, 9, 1, "graph.txt");
    return 0;

}
