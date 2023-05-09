#ifndef CSE305_PROJECT_GRAPH_GEN_H
#define CSE305_PROJECT_GRAPH_GEN_H

#include "graph.h"
#include <string>

class GraphGenerator {
public:
    /**
     * Generate a graph with n vertices and m edges.
     * @param n Number of vertices.
     * @param m Number of edges.
     * @return Graph with n vertices and m edges.
     */
    static Graph generateGraph(int n, int m);

    /**
     * Generate numGraphs graphs with n vertices and m edges, and save them to a file.
     * @param n Number of vertices.
     * @param m Number of edges.
     * @param numGraphs Number of graphs to generate.
     * @param filename Name of the file to save the graphs to.
     */
    static std::vector<Graph> generateSaveGraphs(int n, int m, int numGraphs, std::string filename);

    /**
     * Generate a graph with n vertices and m edges, and save it to a file.
     * @param n Number of vertices.
     * @param m Number of edges.
     * @param filename Name of the file to save the graph to.
     */
    static void generateSaveGraph(int n, int m, std::string filename);

    /**
     * Load graphs from a file.
     * @param filename Name of the file to load the graphs from.
     * @return Vector of graphs loaded from the file.
     */
    static std::vector<Graph> loadGraphs(std::string filename);
};


#endif //CSE305_PROJECT_GRAPH_GEN_H
