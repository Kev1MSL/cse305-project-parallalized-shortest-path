#include "graph.h"


int Edge::getFrom() {
    return fromVertex;
}

int Edge::getTo() {
    return toVertex;
}

int Edge::getWeight() {
    return edgeWeight;
}

Graph::Graph(int _nbVertices, int _nbEdges) {
    nbVertices = _nbVertices;
    nbEdges = _nbEdges;

    // Allocate memory for the edges
    edges.reserve(nbEdges);

    // Generate graph randomly
    for (int i = 0; i < nbEdges; i++) {

        int fromVertex = rand() % nbVertices;
        int toVertex = rand() % nbVertices;

        // Generate a random weight between 0 and 999
        int edgeWeight = rand() % 1000;

        // Add the edge to the graph
        addEdge(fromVertex, toVertex, edgeWeight);
    }
}

int Graph::getGraphNbEdges() {
    return nbEdges;
}

int Graph::getGraphNbVertices() {
    return nbVertices;
}

void Graph::addEdge(int fromVertex, int toVertex, int edgeWeight) {
    edges.emplace_back(fromVertex, toVertex, edgeWeight);
}

std::vector<Edge> Graph::getEdges() {
    return edges;
}

std::vector<int> Graph::getVertices() {
    return vertices;
}

void Graph::setEdges(std::vector<Edge> _edges) {
    edges = _edges;
}

void Graph::setVertices(int _vertices) {
    // Arrange from 0 to n-1 vertices
    for (int i = 0; i < _vertices; i++) {
        vertices.push_back(i);
    }
}
