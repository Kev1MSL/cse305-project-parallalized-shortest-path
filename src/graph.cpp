#include "graph.h"



int Edge::get_from() const
{
	return from_vertex_;
}

int Edge::get_to() const
{
	return to_vertex_;
}

double Edge::get_weight() const
{
	return edge_weight_;
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
			allEdges.insert(std::make_pair(j, i));
		}
	}
	// Generate graph randomly
	for (int i = 0; i < nbEdges; i++) {
		// Pick a random edge
		if (allEdges.empty()) {
			nbEdges = i;
			break;
		}
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
		// if no more edges to pick, set new nbEdges and break
	}

	computeDegrees();
	createAdjList();
	//createAdjMatrix();
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
	std::cout << "Graph " << nbVertices << ", Vertices " << nbVertices << ", Edges " << nbEdges << std::endl;
	for (int i = 0; i < nbEdges; i++) {
		std::cout << edges[i].get_from() << " " << edges[i].get_to() << " " << edges[i].get_weight() << std::endl;
	}
}

void Graph::saveGraph(FILE* fp, int edgeIndex) {
	fprintf(fp, "Graph %d, Vertices %d, Edges %d\n", edgeIndex, nbVertices, nbEdges);
	for (int i = 0; i < nbEdges; i++) {
		fprintf(fp, "%d %d %f\n", edges[i].get_from(), edges[i].get_to(), edges[i].get_weight());
	}
}

void Graph::computeDegrees() {
	degrees = std::vector<int>(nbVertices, 0);
	for (int i = 0; i < nbEdges; i++) {
		degrees[edges[i].get_from()]++;
	}
}


//void Graph::createAdjMatrix() {
//	adjMatrix = std::vector<std::vector<double>>(nbVertices, std::vector<double>(nbVertices, 0));
//
//	for (size_t i = 0; i < edges.size(); i++) {
//		adjMatrix[edges[i].get_from()][edges[i].get_to()] = edges[i].get_weight();
//
//	}
//}

void Graph::createAdjList()
{
	// Initialize the adjacency list
	this->adjList = std::map<int, std::vector<std::pair<int, double>>>();

	// Add the edges to the adjacency list
	for (const Edge& edge : this->edges)
	{
		adjList[edge.get_from()].push_back(std::make_pair(edge.get_to(), edge.get_weight()));
	}

}

//std::vector<std::vector<double>> Graph::getAdjMatrix() {
//	return adjMatrix;
//}
double Graph::getEdgeWeight(const int from, const int to) const
{
	if (adjList.contains(from))
	{
		const auto it = std::ranges::find_if(adjList.at(from), [to](const std::pair<int, double>& p) {return p.first == to; });
		if (it != adjList.at(from).end())
		{
			return it->second;
		}
	}
	return 0.;
	// If we are sure that the edge exists, we can use the following line instead of the above code:
	// return adjList[from][to];
}

bool Graph::areNeighbors(int v1, int v2) {
	return Graph::getEdgeWeight(v1, v2) != 0.;
}

//void Graph::printAdjMatrix() {
//	printf("Adjacency Matrix:\n");
//	for (size_t i = 0; i < adjMatrix.size(); i++) {
//		for (size_t j = 0; j < adjMatrix[i].size(); j++) {
//			std::cout << adjMatrix[i][j] << " ";
//		}
//		std::cout << ',' << std::endl;
//	}
//}

void Graph::printAdjList()
{
	std::cout << "Adjacency List:" << std::endl;
	for (const std::pair<int, std::vector<std::pair<int, double>>> p : adjList)
	{
		std::cout << p.first << ": ";
		for (const std::pair<int, double>& p2 : p.second)
		{
			std::cout << p2.first << " ";
		}
		std::cout << std::endl;
	}
}

std::vector<int> Graph::getGraphDegrees() {
	return degrees;
}


std::set<int> Graph::get_vertex_neighbors(int v) {
	/*std::set<int> neighbours;
	for (size_t i = 0; i < adjMatrix[v].size(); i++) {
		if (adjMatrix[v][i] != 0) {
			neighbours.insert(i);
		}
	}
	return neighbours;*/

	std::set<int> neighbors;
	if (adjList.contains(v))
	{
		for (const std::pair<int, double>& p : adjList.at(v))
		{
			neighbors.insert(p.first);
		}
	}
	return neighbors;
}

void Graph::computeMaxDegree() {
	// assuming that degrees are already found.
	maxDegree = *std::max_element(degrees.begin(), degrees.end());
}

int Graph::getMaxDegree() {
	return maxDegree;
}

void Graph::print_graph_info() const
{
	std::cout << "Graph " << nbVertices << ", Vertices " << nbVertices << ", Edges " << nbEdges << std::endl;
	std::cout << "Max degree: " << maxDegree << std::endl;
	std::cout << "Degrees: ";
	for (int i = 0; i < nbVertices; i++)
	{
		std::cout << degrees[i] << " ";
	}
	std::cout << std::endl;
	/*std::cout << "Adjacency Matrix:" << std::endl;
	for (int i = 0; i < nbVertices; i++)
	{
		for (int j = 0; j < nbVertices; j++)
		{
			std::cout << adjMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/
}
