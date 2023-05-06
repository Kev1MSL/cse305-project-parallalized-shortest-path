//generate graph objects from graph.cpp
#include "graph.cpp"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ctime>

std::vector<Graph> generateGraphs(int n, int m, int numGraphs) {
    std::vector<Graph> graphs;
    graphs.reserve(numGraphs);
    for (int i = 0; i < numGraphs; i++) {
        graphs.emplace_back(n, m);
    }
    return graphs;
}

void generate_save_Graphs(int n, int m, int numGraphs, std::string filename) {
    std::vector<Graph> graphs = generateGraphs(n, m, numGraphs);
    FILE *fp = fopen(filename.c_str(), "w");
    for (int i = 0; i < numGraphs; i++) {
        graphs[i].save(fp);
    }
    fclose(fp);
}

std::vector<Graph> loadGraphs(std::string filename) {
    std::vector<Graph> graphs;
    FILE *fp = fopen(filename.c_str(), "r");
    while (true) {
        Graph g;
        if (g.load(fp)) {
            graphs.push_back(g);
        } else {
            break;
        }
    }
    fclose(fp);
    return graphs;
}
