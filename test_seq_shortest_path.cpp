//
// Created by Kevin on 18/05/2023.
//

#include <iostream>
#include "src/graph_gen.h"
#include "src/deltastep_seq.h"
#include "src/seq_shortest_path.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./seq_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex>\n");
        return -1;
    }

    // Get the arguments
	const std::string path = argv[1];
    const int source_vertex = strtol(argv[2], nullptr, 10);
    const int destination_vertex = strtol(argv[3], nullptr, 10);
    Graph graph = GraphGenerator::loadGraphs(path)[0];
    graph.printGraph();
    graph.printAdjMatrix();

    auto delta_step_seq = DeltaStepSequential(graph, source_vertex);
    delta_step_seq.solve_light_heavy();
    delta_step_seq.print_solution();

    DijkstraFibonacciHeap::dijkstra(graph, source_vertex, destination_vertex);


    return 0;
}