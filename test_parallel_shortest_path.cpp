#include "src/graph_gen.h"
#include "src/deltastep_parallel.h"
#include "src/seq_shortest_path.h"


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
	graph.printGraph();
	graph.printAdjMatrix();

	std::cout << "--------------------------------------------------" << std::endl;

	if (source_vertex < 0 || source_vertex >= graph.getGraphNbVertices())
	{
		std::cerr << "[ERROR] Source vertex is out of bounds" << std::endl;
		return -1;
	}
	auto delta_step_par = DeltaStepParallel(graph, source_vertex, thread_number, is_verbose);
	delta_step_par.solve();
	delta_step_par.print_solution();

	DijkstraFibonacciHeap::dijkstra(graph, source_vertex, destination_vertex);
	return 0;
}

