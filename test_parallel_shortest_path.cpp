#include "src/graph_gen.h"
#include "src/deltastep_parallel.h"
#include "src/seq_shortest_path.h"
#include <cstring>


int main(const int argc, char* argv[])
{
	 if (argc < 5) {
	 	std::cerr << "Usage: ./parallel_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex> <thread_number>" << std::endl;
	 	return -1;
	 }
	 bool is_verbose = false;
	 if (argc == 6)
	 {
	 	if (strcmp(argv[5], "-v") == 0)
	 	{
	 		is_verbose = true;
	 	}
	 }

	// Get the arguments
	const std::string path = argv[1];
	const int source_vertex = strtol(argv[2], nullptr, 10);
	const int destination_vertex = strtol(argv[3], nullptr, 10);
	const int thread_number = strtol(argv[4], nullptr, 10);
	Graph graph = GraphGenerator::loadGraphs(path)[0];
	if (is_verbose)
	{
		graph.printGraph();
		graph.printAdjList();
	}else
	{
		graph.print_graph_info();
	}

	std::cout << "--------------------------------------------------" << std::endl;

	if (source_vertex < 0 || source_vertex >= graph.getGraphNbVertices())
	{
		std::cerr << "[ERROR] Source vertex is out of bounds" << std::endl;
		return -1;
	}

	auto delta_step_par = DeltaStepParallel(graph, source_vertex, thread_number, is_verbose);
	auto start = std::chrono::high_resolution_clock::now();
	delta_step_par.solve();
	auto end = std::chrono::high_resolution_clock::now();
	//delta_step_par.print_solution();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Solving time for DeltaStepParallel: " << duration.count() << " microseconds" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	std::vector<double> dijkstra_sol = DijkstraFibonacciHeap::dijkstra(graph, source_vertex, destination_vertex);
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Solving time for DijkstraFibonacciHeap: " << duration.count() << " microseconds" << std::endl;

	// Check the correctness of the solution:
	int is_correct_counter = 0;
	for (int i = 0; i < graph.getGraphNbVertices(); i++)
	{
		if (delta_step_par.get_dist(i) != dijkstra_sol[i])
		{
			++is_correct_counter;
			std::cout << "Delta Step solution is not correct" << std::endl;
			std::cout << "Vertex " << i << " has distance " << delta_step_par.get_dist(i) << " instead of " << dijkstra_sol[i] << std::endl;
		}
	}

	if (is_correct_counter == 0)
	{
		std::cout << "Delta Step Parallel solution is correct!" << std::endl;
	}else
	{
		std::cout << "Delta Step solution is not correct for " << is_correct_counter << " value(s)." << std::endl;
	}



	return 0;
}

