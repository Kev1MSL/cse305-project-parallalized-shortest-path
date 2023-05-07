#ifndef CSE305_PROJECT_GRAPH_H
#define CSE305_PROJECT_GRAPH_H

#include <vector>
#include <limits>
#include <cmath>
#include <random>
#include <iostream>
#include <set>

/**
 * Class representing an edge in the graph.
 */
class Edge {
private:

    // Source vertex
    int fromVertex;

    // Destination vertex
    int toVertex;

    // Weight of the edge
    double edgeWeight;
public:
    /**
     * Constructor for an edge.
     * @param _from From vertex.
     * @param _to   To vertex.
     * @param _weight  Weight of the edge.
     */
    Edge(int _from, int _to, double _weight) : fromVertex(_from), toVertex(_to), edgeWeight(_weight) {}

    /**
     * Get the source vertex.
     * @return Source vertex.
     */
    int getFrom();

    /**
     * Get the destination vertex.
     * @return Destination vertex.
     */
    int getTo();

    /**
     * Get the weight of the edge.
     * @return Weight of the edge.
     */
    double getWeight();

    /**
     * Give ordering for a set of edges
     * @param other Edge to compare to.
    */
    bool operator<(const Edge &other) const {
        if (fromVertex < other.fromVertex) {
            return true;
        } else if (fromVertex == other.fromVertex) {
            if (toVertex < other.toVertex) {
                return true;
            } else if (toVertex == other.toVertex) {
                if (edgeWeight < other.edgeWeight) {
                    return true;
                }
            }
        }
        return false;
    }

};

/**
 * Class representing a graph.
 */
class Graph {
private:
    // Number of vertices
    int nbVertices;

    // Number of edges
    int nbEdges;

    // Edges
    std::vector<Edge> edges;

    // Adjancy list:
    std::vector<std::vector<std::pair<int, double>>> adjList;

    // Vertices
    std::vector<int> vertices;

    std::vector<int> degrees;

public:
    /**
     * Empty constructor for a graph, used for loading graphs from file.
     */
    Graph() {};

    /**
     * Constructor for a graph.
     * @param _nbVertices Number of vertices in the graph.
     * @param _nbEdges Number of edges in the graph.
     */
    Graph(int _nbVertices, int _nbEdges);

    /**
     * Constructor for a graph loaded from a file.
     * @param _nbVertices Number of vertices in the graph.
     * @param _nbEdges Number of edges in the graph.
     * @param _edges Edges in the graph as a vector of Edge objects.
     * @param _vertices Vertices in the graph as a vector of integers.
     */
    Graph(int _nbVertices, int _nbEdges, std::vector<Edge> _edges, std::vector<int> _vertices):
    nbVertices(_nbVertices), nbEdges(_nbEdges), edges(_edges), vertices(_vertices) {
        computeDegrees();
        createAdjList();
    };

    /**
     * Get the number of vertices in the graph.
     * @return Number of vertices in the graph.
     */
    int getGraphNbVertices();

    /**
     * Get the number of edges in the graph.
     * @return Number of edges in the graph.
     */
    int getGraphNbEdges();

    /**
     * Get the edges in the graph.
     * @return Edges in the graph as a vector of Edge objects.
     */
    std::vector<Edge> getEdges();

    /**
     * Get the vertices in the graph.
     * @return Vertices in the graph as a vector of integers.
     */
    std::vector<int> getVertices();

    /**
     * Add an edge to the graph.
     * @param fromVertex Source vertex.
     * @param toVertex Destination vertex.
     * @param edgeWeight Weight of the edge.
     */
    void addEdge(int fromVertex, int toVertex, double edgeWeight);

    /**
     * Set the edges in the graph, used for loading graphs from file.
     * @param _edges Edges in the graph as a vector of Edge objects.
     */
    void setEdges(std::vector<Edge> _edges);

    /**
     * Set the vertices in the graph, used for loading graphs from file.
     * @param _vertices Vertices in the graph as a vector of integers.
     */
    void setVertices(int _vertices);
    /**
     * Print the graph.
     */
    void printGraph();

    /**
     * Save the graph to a file.
     * @param fp File pointer to save the graph to.
     * @param edgeIndex Index of the edge.
     */
    void saveGraph(FILE *fp, int edgeIndex);

    /**
     * Compute the degrees of the vertices in the graph.
     */
    void computeDegrees();



    /**
     * Are neighbors:
     * @param v1 vertex 1
     * @param v2 vertex 2
    */
    bool areNeighbors(int v1, int v2);

    /**
     * create adjacency list for dijkstra
    */
    void createAdjList();
};


#endif //CSE305_PROJECT_GRAPH_H
