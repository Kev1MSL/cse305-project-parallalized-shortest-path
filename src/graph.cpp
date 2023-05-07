#include "graph.h"


int Edge::getFrom() {
    return fromVertex;
}

int Edge::getTo() {
    return toVertex;
}

double Edge::getWeight() {
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

    // create a set of all possible edges:
    std::set<std::pair<int, int>> allEdges;
    for (int i = 0; i < nbVertices; i++) {
        for (int j = i + 1; j < nbVertices; j++) {
            allEdges.insert(std::make_pair(i, j));
        }
    }
    // Generate graph randomly
    for (int i = 0; i < nbEdges; i++) {
        // Pick a random edge
        int edgeIndex = rand() % allEdges.size();
        auto it = allEdges.begin();
        std::advance(it, edgeIndex);
        int fromVertex = it->first;
        int toVertex = it->second;
        allEdges.erase(it);

        // Generate a random weight
        double edgeWeight = dist(gen);

        // Add the edge to the graph
        addEdge(fromVertex, toVertex, edgeWeight);
    }

    computeDegrees();
    createAdjList();
}

int Graph::getGraphNbEdges() {
    return nbEdges;
}

int Graph::getGraphNbVertices() {
    return nbVertices;
}

void Graph::addEdge(int fromVertex, int toVertex, double edgeWeight) {
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
        fprintf(fp, "%d %d %f\n", edges[i].getFrom(), edges[i].getTo(), edges[i].getWeight());
    }
}

void Graph::computeDegrees() {
    degrees = std::vector<int>(nbVertices, 0);
    for (int i = 0; i < nbEdges; i++) {
        degrees[edges[i].getFrom()]++;
    }
}




//create adjacency list
void Graph::createAdjList(){
    //create adjacency list
    //iterate through edges
    for(int i = 0; i < edges.size(); i++){
        //add edge to adjacency list
        std::pair<int,double> p = std::make_pair(edges[i].getTo(), edges[i].getWeight());
        printf("pair: %d %f\n", p.first, p.second);
        adjList[edges[i].getFrom()].push_back(p);
    }
}
    
bool Graph::areNeighbors(int v1, int v2){
    //iterate through adjacency list of v1
    for(int i = 0; i < adjList[v1].size(); i++){
        //if v2 is in adjacency list of v1
        if(adjList[v1][i].first == v2){
            return true;
        }
    }
    return false;
}

