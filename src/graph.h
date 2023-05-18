#ifndef CSE305_PROJECT_GRAPH_H
#define CSE305_PROJECT_GRAPH_H

#include <vector>
#include <iostream>
#include <set>
#include <random>
/**
 * Class representing an edge in the graph.
 */
class Edge {
private:

    // Source vertex
    int from_vertex_;

    // Destination vertex
    int to_vertex_;

    // Weight of the edge
    double edge_weight_;
public:
    /**
     * Constructor for an edge.
     * @param from From vertex.
     * @param to   To vertex.
     * @param weight  Weight of the edge.
     */
    Edge(const int from, const int to, const double weight) : from_vertex_(from), to_vertex_(to), edge_weight_(weight) {}

    /**
     * Get the source vertex.
     * @return Source vertex.
     */
    int get_from();

    /**
     * Get the destination vertex.
     * @return Destination vertex.
     */
    int get_to();

    /**
     * Get the weight of the edge.
     * @return Weight of the edge.
     */
    double get_weight();

    /**
     * Give ordering for a set of edges
     * @param other Edge to compare to.
    */
    bool operator<(const Edge &other) const {
        if (from_vertex_ < other.from_vertex_) {
            return true;
        } if (from_vertex_ == other.from_vertex_) {
            if (to_vertex_ < other.to_vertex_) {
                return true;
            } if (to_vertex_ == other.to_vertex_) {
                if (edge_weight_ < other.edge_weight_) {
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

    // Adjancy matrix
    std::vector<std::vector<double>> adjMatrix;


    // Vertices
    std::vector<int> vertices;

    std::vector<int> degrees;

    int maxDegree;
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
        computeMaxDegree();
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
     * Checks whether two vertices are neighbors.
     * @param from Vertex 1
     * @param to Vertex 2
     * @return true if v1 and v2 are neighbors, false otherwise.
    */
    bool areNeighbors(int from, int to);

    /**
     * Compute the adjacency list of the graph, used for djikstra's algorithm.
    */
    void createAdjList();

    /**
     * Get the weight of an edge.
     * @param from Vertex 1.
     * @param to Vertex 2.
     * @return Weight of the edge.
    */
    double getEdgeWeight(int from, int to);

    /**
     * Print adjacency matrix
    */
    void printAdjMatrix();

    /**
     * Get degrees of vertices
    */
    std::vector<int> getGraphDegrees();

    /**
     * Get neighbors of a vertex
    */
    std::set<int> getGraphNeighbours(int v);

    /*
    * Get adjacency matrix
    */
    std::vector<std::vector<double>> getAdjMatrix();

    /*
    * compute maximum degree of graph
    */
    void computeMaxDegree();

    /*
    * get maximum degree of graph
    */
    int getMaxDegree();

    /**
     * @brief Print graph info to the console. 
     */
    void print_graph_info() const;
};



#endif //CSE305_PROJECT_GRAPH_H
