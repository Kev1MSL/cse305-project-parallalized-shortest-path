//
// Created by Kevin on 18/05/2023.
//

#include <chrono>

#include "src/graph_gen.h"
#include "src/deltastep_seq.h"
#include "src/seq_shortest_path.h"
#include <cstring>


int main(const int argc, char* argv[]) {
	if (argc < 4) {
		std::cerr << "Usage: ./seq_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex>" << std::endl;
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
	if (is_verbose)
	{
		graph.printGraph();
		graph.printAdjList();
	}


	std::cout << "--------------------------------------------------" << std::endl;

	if (source_vertex < 0 || source_vertex >= graph.getGraphNbVertices())
	{
		std::cerr << "[ERROR] Source vertex is out of bounds" << std::endl;
		return -1;
	}

	//auto delta_step_seq = DeltaStepSequential(graph, source_vertex, is_verbose);
	//auto start = std::chrono::high_resolution_clock::now();
	//delta_step_seq.solve();
	//auto end = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	//std::cout << std::endl << "Basic Delta Step solution: " << std::endl;
	//delta_step_seq.print_solution();
	//std::cout << "Solving time for Basic Delta Step Sequential: " << duration.count() << " microseconds" << std::endl;

	auto delta_step_seq_lh = DeltaStepSequential(graph, source_vertex);
	auto start = std::chrono::high_resolution_clock::now();
	delta_step_seq_lh.solve_light_heavy();
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	//std::cout << std::endl << "Light Heavy Delta Step solution: " << std::endl;
	//delta_step_seq_lh.print_solution();
	std::cout << "Solving time for Light Heavy Delta Step Sequential: " << duration.count() << " microseconds" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	const std::vector<double> dijkstra_sol = DijkstraFibonacciHeap::dijkstra(graph, source_vertex, destination_vertex);
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Solving time for DijkstraFibonacciHeap: " << duration.count() << " microseconds" << std::endl;


	// Check the correctness of the solution:
	int is_correct_counter = 0;
	for (int i = 0; i < graph.getGraphNbVertices(); i++)
	{
		if (delta_step_seq_lh.get_dist(i) != dijkstra_sol[i])
		{
			++is_correct_counter;
			std::cout << "Delta Step solution is not correct" << std::endl;
			std::cout << "Vertex " << i << " has distance " << delta_step_seq_lh.get_dist(i) << " instead of " << dijkstra_sol[i] << std::endl;
		}
	}

	if (is_correct_counter == 0)
	{
		std::cout << "Delta Step solution is correct!" << std::endl;
	}
	else
	{
		std::cout << "Delta Step solution is not correct for " << is_correct_counter << " value(s)." << std::endl;
	}


	return 0;
}