//
// Created by Kevin on 18/05/2023.
//

#include "src/graph_gen.h"
#include "src/deltastep_seq.h"
#include "src/seq_shortest_path.h"


int main(const int argc, char* argv[]) {
	if (argc < 4) {
		fprintf(stderr, "Usage: ./seq_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex>\n");
		return -1;
	}
	bool is_verbose = false;
	if (argc == 5)
	{
		if (strcmp(argv[4], "-v") == 0)
		{
			is_verbose = true;
		}
	}

	// Get the arguments
	const std::string path = argv[1];
	const int source_vertex = strtol(argv[2], nullptr, 10);
	const int destination_vertex = strtol(argv[3], nullptr, 10);
	Graph graph = GraphGenerator::loadGraphs(path)[0];
	graph.printGraph();
	graph.printAdjMatrix();

	if (source_vertex < 0 || source_vertex >= graph.getGraphNbVertices())
	{
		std::cerr << "[ERROR] Source vertex is out of bounds" << std::endl;
		return -1;
	}

	auto delta_step_seq = DeltaStepSequential(graph, source_vertex, is_verbose);
	delta_step_seq.solve();
	delta_step_seq.print_solution();

    auto delta_step_seq_lh = DeltaStepSequential(graph, source_vertex);
    
    delta_step_seq_lh.solve_light_heavy();
    printf("Delta step sequential light heavy\n");
    delta_step_seq_lh.print_solution();

    DijkstraFibonacciHeap::dijkstra(graph, source_vertex, destination_vertex);

	return 0;
}