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

void generateSaveGraphs(int n, int m, int numGraphs, std::string filename) {
    std::vector<Graph> graphs = generateGraphs(n, m, numGraphs);
    FILE *fp = fopen(filename.c_str(), "w");
    for (int i = 0; i < numGraphs; i++) {
        graphs[i].save(fp, i);
    }
    fclose(fp);
}

std::vector<Graph> loadGraphs(std::string filename) {
    std::vector<Graph> graphs;
    FILE *fp = fopen(filename.c_str(), "r");
    int n, m;
    while (fscanf(fp, "%d %d", &n, &m) != EOF) {
        graphs.emplace_back(n, m);
        graphs.back().load(fp);
    }
    fclose(fp);
    return graphs;
}


int main() {
    srand(time(NULL));
    generateSaveGraphs(100, 1000, 100, "test_100v_1000e_100g.txt");
    // std::vector<Graph> graphs = loadGraphs("test_100v_1000e_100g.txt");
    // for (int i = 0; i < graphs.size(); i++) {
    //     graphs[i].printGraph();
    // }
    return 0;
}