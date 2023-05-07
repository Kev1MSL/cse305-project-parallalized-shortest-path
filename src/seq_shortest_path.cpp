#include <iostream>
#include "graph_gen.h"
#include "graph.h"
#include <iostream>
#include <boost/heap/fibonacci_heap.hpp> // Include the fibonacci heap library

#define INF std::numeric_limits<int>::max()
// Dijkstra's algorithm sequentially with fibonacci heap


/**
 * Dijkstra's algorithm for finding the shortest path from a source vertex to a destination vertex.
 * @param graph Graph to run the algorithm on.
 * @param source Source vertex.
 * @param destination Destination vertex.
 */
void dijkstra(Graph graph, int source, int destination) {
    // Initialize the distance array:
    std::vector<double> distance(graph.getGraphNbVertices(), INF);
    distance[source] = 0;
    // Initialize the fibonacci heap:
    boost::heap::fibonacci_heap<std::pair<int, int>, boost::heap::compare<std::greater<std::pair<int, int> > > > heap;
    
    // Initialize the heap:
    heap.push(std::make_pair(0, source));

    // Initialize the visited array:
    std::vector<bool> visited(graph.getGraphNbVertices(), false);

    // Initialize the previous array:
    std::vector<int> previous(graph.getGraphNbVertices(), -1);

    // Run the algorithm:
    while (!heap.empty()) {
        // Get the vertex with the minimum distance:
        int vertex = heap.top().second;
        heap.pop();

        // If the vertex has been visited, continue:
        if (visited[vertex]) {
            continue;
        }

        // Mark the vertex as visited:
        visited[vertex] = true;
        // Iterate through the neighbors of the vertex:
        for (int i = 0; i < graph.getGraphNbVertices(); i++) {
            // If the vertex is not a neighbor, continue:
            if (!graph.areNeighbors(vertex, i)) {
                continue;
            }

            // If the neighbor has been visited, continue:
            if (visited[i]) {
                continue;
            }

            // Get the weight of the edge:
            // find in edgeset the edge with fromVertex = vertex and toVertex = i
            
            
            double weight = graph.getEdgeWeight(vertex, i);

            // If the distance to the neighbor is greater than the distance to the current vertex plus the weight of the edge, update the distance:
            if (distance[i] > distance[vertex] + weight) {
                distance[i] = distance[vertex] + weight;
                heap.push(std::make_pair(distance[i], i));
                previous[i] = vertex;
            }
        }
    }
    // Print the distance:
    std::cout << "Distance from " << source << " to " << destination << ": " << distance[destination] << std::endl;
}

/**
 * Test the algorithm.
 * argv[1]: path to the graph file
 * argv[2]: source vertex
 * argv[3]: destination vertex
*/
int main(int argc, char* argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: ./seq_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex>\n");
        return -1;
    }

    // Get the arguments
    std::string path = argv[1];
    int sourceVertex = atoi(argv[2]);
    int destinationVertex = atoi(argv[3]);

    // Load the graph from the file
    Graph graph = GraphGenerator::loadGraphs(path)[0];
    graph.printGraph();

    // Run the algorithm:
    dijkstra(graph, sourceVertex, destinationVertex);

    return 0;
}