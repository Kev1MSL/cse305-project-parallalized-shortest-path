#ifndef CSE305_PROJECT_PARALLEL_SHORTEST_PATHS_H
#define CSE305_PROJECT_PARALLEL_SHORTEST_PATHS_H

#include "graph.h"
#include "graph_gen.h"
#include <algorithm>
#include <set>
#include <numeric>
#include <cmath>
#include <iostream>
#include <thread>


using namespace std;
using Request_vect = vector<tuple<int, int, double>>;
class ParallelSSSP {
    private:
    size_t num_threads;
    Graph graph;
    int source;
    vector<double> dist;
    vector<int> pred;
    set<int> bucket_inf;
    vector<set<int>> buckets;
    double delta;
    vector<vector<int>> LightEdges;
    vector<vector<int>> HeavyEdges;
    vector<vector<double>> adjMatrix;
    int k = 0; //starting bucket counter




    public:
    
    ParallelSSSP(size_t _thread_count, Graph _graph, int _source);

    /**
    * @brief Get the find requests by generation in parallel chunks
    * @param request_light Set of light edges
    * @param request_heavy Set of heavy edges
    * @param begin Begin index
    * @param chunk_size Chunk size
    * @return Distance vector
    */
    void findRequests(Request_vect* request_light, Request_vect* request_heavy, size_t begin, size_t chunk_size);
    
    /**
    * @brief Relax an edge
    * @param u Source vertex
    * @param v Destination vertex
    * @param w Weight of the edge
    * @return Distance vector
    */
    void relax(int u, int v, double w);

    /**
    * @brief Relax a set of edges in parallel
    * @param requests Set of edges to relax
    * @param begin Begin index
    * @param chunk_size Chunk size
    * @return Distance vector
    */
    void relaxRequestsParallel(Request_vect* requests, size_t begin, size_t chunk_size);

    /**
    * @brief Pre computation light and heavy edges
    * @return Distance vector
    */
    void computeLightHeavyEdges();

    /**
    * @brief Solve the SSSP problem
    * @return Distance vector
    */
    void solveSSSP();

    /**
     * @brief Get the distance vector
     * @return Distance vector
    */
    vector<double> getDist();

    /**
     * @brief Get the predecessor vector
     * @return Predecessor vector
    */
    vector<int> getPred();


};

#endif //CSE305_PROJECT_PARALLEL_SHORTEST_PATHS_H