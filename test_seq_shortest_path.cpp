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
        std::string path = argv[1];
    int sourceVertex = atoi(argv[2]);
    int destinationVertex = atoi(argv[3]);
    Graph graph = GraphGenerator::loadGraphs(path)[0];
    graph.printGraph();
    graph.printAdjMatrix();

    DeltaStepSequential delta_step_seq = DeltaStepSequential(graph, sourceVertex);
    delta_step_seq.solve_light_heavy();
    delta_step_seq.print_solution();

    DijkstraFibonacciHeap::dijkstra(graph, sourceVertex, destinationVertex);


    return 0;
}

//int main(int argc, char* argv[]) {
//
//    if (argc != 4) {
//        fprintf(stderr, "Usage: ./seq_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex>\n");
//        return -1;
//    }
//
//    // Get the arguments
//    std::string path = argv[1];
//    int sourceVertex = atoi(argv[2]);
//    int destinationVertex = atoi(argv[3]);
//    Graph graph = GraphGenerator::loadGraphs(path)[0];
//    graph.printGraph();
//    graph.printAdjMatrix();
//
//    ParallelSSSP p = ParallelSSSP(2, graph, sourceVertex);
//    p.solveSSSP();
//    //print distance and predecessor vectors
//    vector<double> dist = p.getDist();
//    vector<int> pred = p.getPred();
//    //print distance to destination vertex
//    cout << "Distance to destination vertex: " << dist[destinationVertex] << endl;
//    //print path to destination vertex
//
//    return 0;
//}