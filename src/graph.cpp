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

    // Generate the weights uniformly at random between 0 and 1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Generate graph randomly
    for (int i = 0; i < nbEdges; i++) {

        int fromVertex = rand() % nbVertices;
        int toVertex = rand() % nbVertices;

        // Generate a random weight uniformly between 0 and 1
        int edgeWeight = dist(gen);

        // Add the edge to the graph
        addEdge(fromVertex, toVertex, edgeWeight);
    }

    computeDegrees();
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

void Graph::printGraph() {
    for (int i = 0; i < nbEdges; i++) {
        std::cout << edges[i].getFrom() << " " << edges[i].getTo() << " " << edges[i].getWeight() << std::endl;
    }
}

void Graph::saveGraph(FILE *fp, int edgeIndex) {
    fprintf(fp, "Graph %d, Vertices %d, Edges %d\n", edgeIndex, nbVertices, nbEdges);
    for (int i = 0; i < nbEdges; i++) {
        fprintf(fp, "%d %d %d\n", edges[i].getFrom(), edges[i].getTo(), edges[i].getWeight());
    }
}

void Graph::computeDegrees() {
    degrees = std::vector<int>(nbVertices, 0);
    for (int i = 0; i < nbEdges; i++) {
        degrees[edges[i].getFrom()]++;
    }
}