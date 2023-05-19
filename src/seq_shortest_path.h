#include <iostream>
#include "graph_gen.h"
#include "graph.h"
#include <iostream>
#include <boost/heap/fibonacci_heap.hpp> // Include the fibonacci heap library

#define INF std::numeric_limits<int>::max()
// Dijkstra's algorithm sequentially with fibonacci heap

//custom comparator for fibonacci heap
struct CustomComparator { 
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const {
        // Define the custom ordering logic here
        // For example, order by the first element (weight) in ascending order
        return lhs.first > rhs.first;
    }
};
void print_heap(boost::heap::fibonacci_heap<std::pair<int, int>, boost::heap::compare<CustomComparator>> heap) {
    printf("Heap: ");
    while (!heap.empty()) {
        printf("%d ", heap.top().second);
        heap.pop();
    }
    printf("\n");
};


class DijkstraFibonacciHeap {
public:
    /**
 * Dijkstra's algorithm for finding the shortest path from a source vertex to a destination vertex.
 * @param graph Graph to run the algorithm on.
 * @param source Source vertex.
 * @param destination Destination vertex.
 */
    static void dijkstra(Graph graph, int source, int destination) {
        // Initialize the distance array:
        std::vector<double> distance(graph.getGraphNbVertices(), INF);
        
        // Initialize the fibonacci heap:
        boost::heap::fibonacci_heap<std::pair<int, int>, boost::heap::compare<CustomComparator>> heap;
        // Initialize the heap:
        for (int i = 0; i < graph.getGraphNbVertices(); i++) {
            heap.push(std::make_pair(distance[i], i));
        }

        // Initialize the visited array:
        // Initialize the previous array:
        std::vector<int> previous(graph.getGraphNbVertices(), -1);
        distance[source] = 0;

        // Run the dijkstra algorithm:
        while (!heap.empty()) {
            // Get the vertex with the minimum distance:
            int u = heap.top().second;
            heap.pop();

            // Get the neighbors of the vertex:
            std::set<int> neighbors = graph.getGraphNeighbours(u);

            // Iterate through the neighbors:
            for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
                int v = *it;
                double alt = distance[u] + graph.getEdgeWeight(u, v);
                if (alt < distance[v]) {
                    distance[v] = alt;
                    previous[v] = u;
                    // Update the heap:
                    heap.push(std::make_pair(distance[v], v));
                }
            }
        }
        
        
        std::cout << std::endl << "Dijkstra output" << std::endl;
        std::cout << "Distance from " << source << " to " << destination << ": " << distance[destination] << std::endl;

        // other distances:
        std::cout << "All distances: " << std::endl;
        for (int i = 0; i < graph.getGraphNbVertices(); i++) {
            std::cout << "Distance from " << source << " to " << i << ": " << distance[i] << std::endl;
        }
    }
};

/**
 * Test the algorithm.
 * argv[1]: path to the graph file
 * argv[2]: source vertex
 * argv[3]: destination vertex
*/
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
//
//    // Load the graph from the file
//    Graph graph = GraphGenerator::loadGraphs(path)[0];
//    graph.printGraph();
//    graph.printAdjMatrix();
//    // Run the algorithm:
//    dijkstra(graph, sourceVertex, destinationVertex);
//
//    return 0;
//}