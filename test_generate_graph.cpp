#include "src/graph_gen.h"

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
    GraphGenerator::generateSaveGraphs(n, m, numGraphs, "graph.txt");
    return 0;

}