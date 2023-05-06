#ifndef CSE305_PROJECT_GRAPH_H
#define CSE305_PROJECT_GRAPH_H

#include <vector>
#include <limits>
#include <cmath>

/**
 * Class representing an edge in the graph.
 */
class Edge {
public:
    /**
     * Constructor for an edge.
     * @param _from From vertex.
     * @param _to   To vertex.
     * @param _weight  Weight of the edge.
     */
    Edge(int _from, int _to, int _weight) : fromVertex(_from), toVertex(_to), edgeWeight(_weight) {}

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
    int getWeight();

private:

    // Source vertex
    int fromVertex;

    // Destination vertex
    int toVertex;

    // Weight of the edge
    int edgeWeight;
};

/**
 * Class representing a graph.
 */
class Graph {
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
    void addEdge(int fromVertex, int toVertex, int edgeWeight);

    /**
     * Set the edges in the graph, used for loading graphs from file.
     * @param _edges Edges in the graph as a vector of Edge objects.
     */
    void setEdges(std::vector<Edge> _edges);

    /**
     * Set the vertices in the graph, used for loading graphs from file.
     * @param _vertices Vertices in the graph as a vector of integers.
     */
    void setVertices(std::vector<int> _vertices);


private:
    // Number of vertices
    int nb_vertices;

    // Number of edges
    int nb_edges;

    // Edges
    std::vector<Edge> edges;

    // Vertices
    std::vector<int> vertices;
};


#endif //CSE305_PROJECT_GRAPH_H
