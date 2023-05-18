#include "parallel_shortest_paths.h"

// DEBUG purposes
void printBucket(vector<set<int>> buckets, int k){
    printf("printing buckets[%d] of size %lu:\n", k, buckets[k].size());
    for (auto it = buckets[k].begin(); it != buckets[k].end(); it++){
        printf("%d ", *it);
    }
    printf("\n");
}
void printAllBuckets(vector<set<int>> buckets){
    for (size_t i = 0; i < buckets.size(); i++){
        printBucket(buckets, i);
    }
}
// ----------------

ParallelSSSP:: ParallelSSSP(size_t _thread_count, Graph _graph, int _source){
    num_threads = _thread_count;
    graph = _graph;
    source = _source;
    graph.computeDegrees();
    graph.createAdjList(); //actually a matrix
    graph.computeMaxDegree();
    delta = 1; // 1/ (static_cast<double>(graph.getMaxDegree()) + 1)
    dist.resize(graph.getGraphNbVertices());
    pred.resize(graph.getGraphNbVertices());
    LightEdges.resize(graph.getGraphNbVertices());
    HeavyEdges.resize(graph.getGraphNbVertices());
    for (int i = 0; i < graph.getGraphNbVertices(); i++) {
        dist[i] = graph.getGraphNbVertices(); // set all distances to infinity, as all weights are less than 1, infinity is the number of vertices
        pred[i] = -1;
    }
    
    
    adjMatrix = graph.getAdjMatrix();
    ParallelSSSP::computeLightHeavyEdges(); // define N^l_v & N^h_v
    //print out all of the light and heavy edges
    for (int i = 0; i < graph.getGraphNbVertices(); i++) {
        cout << "Light Edges for vertex " << i << ": ";
        for (size_t j = 0; j < LightEdges[i].size(); j++) {
            cout << LightEdges[i][j] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < graph.getGraphNbVertices(); i++) {
        cout << "Heavy Edges for vertex " << i << ": ";
        for (size_t j = 0; j < HeavyEdges[i].size(); j++) {
            cout << HeavyEdges[i][j] << " ";
        }
        cout << endl;
    }
    //initialize buckets
    printf("delta: %f\n", delta);
    printf("number of vertices: %d\n", graph.getGraphNbVertices());
    printf("to resize: %d\n", graph.getGraphNbVertices()/delta + 1);
    buckets.resize(graph.getGraphNbVertices()/delta + 1);
    //set bucket infinity B_inf - {s}
    printf("buckets size: %lu\n", buckets.size());
    for (size_t i = 0; i < graph.getGraphNbVertices(); i++){
        if (i != source){
            buckets[buckets.size()-1].insert(i);
        }
    }
    // as all the weights in (0, 1), the max path distance should be less than the number of vertices
    // assuming no other distances are 0
    buckets[0].insert(source); //insert source vertex into bucket 0 
    dist[source] = 0;
    pred[source] = source;
    printf("starting buckets:\n");
    printAllBuckets(buckets);
    

}

void ParallelSSSP::findRequests(Request_vect* request_light, Request_vect* request_heavy, size_t begin, size_t end){
    for (int v = begin; v <= end; v++){
        //GenRequests 
        
        buckets[k].erase(v); //remove v from bucket k
        printf("erased %d from bucket %d\n", v, k);
        printBucket(buckets, k);
        //print out all of buckets k 
        
        for (size_t i = 0; i < LightEdges[v].size(); i++){
            int u = LightEdges[v][i]; //insert light neighbor requests
            //print triple
            request_light->push_back(std::make_tuple(v, u, graph.getEdgeWeight(v, u))); //insert (v, u, w) tuple into request_light
            //insertions should have no conflict as long as v is unique and partition is disjoint.
        }
        for (size_t i = 0; i < HeavyEdges[v].size(); i++){
            int u = HeavyEdges[v][i]; //insert heavy neighbor requests
            request_heavy->push_back(std::make_tuple(v, u, graph.getEdgeWeight(v, u)));
        }
    }

}
void ParallelSSSP::relax(int u, int v, double w) {
    //distance from source through u to v vs distance already known to v.
    //as long as relaxation parallel is will partitioned, no conflict should occur.
    printf("performing relaxation on edge (%d, %d) with weight %f\n", u, v, w);
    double tentative = dist[u] + w;
    printf("dist of %d: %f\n", v, dist[v]);
    printf("tentative: %f\n", tentative);
    if (dist[v] > tentative){
        int i = floor(dist[v]/delta);
        int j = floor(tentative/delta);
        printf("moving %d from bucket %d to bucket %d\n", v, i, j);
        buckets[i].erase(v);
        buckets[j].insert(v);
        dist[v] = tentative;
        pred[v] = u;
        printf("updated dist of %d: %f\n", v, dist[v]);
    }

}

void ParallelSSSP::relaxRequestsParallel(Request_vect* requests, size_t begin, size_t chunk_size) {
    for (size_t i = begin; i < begin + chunk_size; i++) {
        int u = get<0>(requests->at(i));
        int v = get<1>(requests->at(i));
        double w = get<2>(requests->at(i));
        relax(u, v, w);
    }
    //erase all relaxed requests
    requests->erase(requests->begin() + begin, requests->begin() + begin + chunk_size);
}

void ParallelSSSP::computeLightHeavyEdges() {
    //iterate over adjacency matrix of graph
    for (size_t i = 0; i < adjMatrix.size(); i++) {
        for (size_t j = 0; j < adjMatrix[i].size(); j++) {
            if (adjMatrix[i][j] != 0) {
                if (adjMatrix[i][j] <= delta) {
                    LightEdges[i].push_back(j);
                } else {
                    HeavyEdges[i].push_back(j);
                }
            }
        }
    }
}

void ParallelSSSP::solveSSSP(){
    int l = 0; //debug stop from infinite printing on loop counter.
    while (k < buckets.size() ){
        Request_vect request_heavy, request_light; // (u, v, w) tuples
        while (!buckets[k].empty() && l < 2){
            
            l++;
            //LOOP 1
            // ----------------------------Generate Requests--------------------------------
            //set up proper partition for parallelization of findRequests/ genRequests
            size_t req_threads;
            if (num_threads > buckets[k].size()){
                req_threads = buckets[k].size();
            } else {
                req_threads = num_threads;
            }
            printf("req_threads: %lu\n", req_threads);
            std::thread workers[req_threads - 1];
            size_t chunk_size = buckets[k].size()/req_threads;
            for (size_t i = 0; i < req_threads - 1; i++){
                workers[i] = std::thread(&ParallelSSSP::findRequests, this, &request_light, &request_heavy, i*chunk_size, (i+1)*chunk_size);
            }
            //final modulo clean up thread.

            findRequests(&request_light, &request_heavy, (req_threads - 1)*chunk_size, buckets[k].size());
            for (size_t i = 0; i < req_threads - 1; ++i) {
                workers[i].join();
            }
            //print request_light
            // printf("request_light size: %lu\n", request_light.size());
            // //print every element of request light
            // for (size_t i = 0; i < request_light.size(); i++){
            //     printf("request_light[%lu]: (%d, %d, %f)\n", i, get<0>(request_light[i]), get<1>(request_light[i]), get<2>(request_light[i]));
            // }

            //LOOP 2
            // ----------------------------Relax Light Requests--------------------------------
            //set up proper partition for parallelization of relaxRequests
            if (!request_light.empty()){
                size_t relax_threads;
                if (num_threads > request_light.size()){
                    relax_threads = request_light.size();
                } else {
                    relax_threads = num_threads;
                }
                std::thread workers2[relax_threads - 1];
                size_t chunk_size2 = request_light.size()/relax_threads;
                for (size_t i = 0; i < relax_threads - 1; i++){
                    workers2[i] = std::thread(&ParallelSSSP::relaxRequestsParallel, this, &request_light, i*chunk_size2, chunk_size2);
                }
                //final modulo clean up thread.
                relaxRequestsParallel(&request_light, (relax_threads - 1)*chunk_size2, request_light.size() - (relax_threads - 1)*chunk_size2);
                for (size_t i = 0; i < relax_threads - 1; ++i) {
                    workers2[i].join();
                }
            }
            printAllBuckets(buckets);
            printf("request_light size: %lu\n", request_light.size());
        }

        //Loop 3
        // ----------------------------Relax Heavy Requests--------------------------------
        //set up proper partition for parallelization of relaxRequests
        if (!request_heavy.empty()){
            size_t relax_threads;
            if (num_threads > request_heavy.size()){
                relax_threads = request_heavy.size();
            } else {
                relax_threads = num_threads;
            }
            std::thread workers3[relax_threads - 1];
            size_t chunk_size3 = request_heavy.size()/relax_threads;
            printf("number of thread to run %d\n", relax_threads);
            for (size_t i = 0; i < relax_threads - 1; i++){
                workers3[i] = std::thread(&ParallelSSSP::relaxRequestsParallel, this, &request_heavy, i*chunk_size3, chunk_size3);
            }
            //final modulo clean up thread.
            relaxRequestsParallel(&request_heavy, (relax_threads - 1)*chunk_size3, request_heavy.size() - (relax_threads - 1)*chunk_size3);
            for (size_t i = 0; i < relax_threads - 1; ++i) {
                workers3[i].join();
            }
            printf("Relaxed Heavy Requests\n");
            while (buckets[k].empty() && k < graph.getGraphNbVertices()){
                k++;
            }
        }
        //print all buckets:
        
    }
}
vector<double> ParallelSSSP::getDist(){
    return dist;
}
vector<int> ParallelSSSP::getPred(){
    return pred;
}
int main(int argc, char* argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: ./seq_shortest_path <path_to_graph_file> <source_vertex> <destination_vertex>\n");
        return -1;
    }

    // Get the arguments
    std::string path = argv[1];
    int sourceVertex = atoi(argv[2]);
    int destinationVertex = atoi(argv[3]);
    Graph graph = GraphGenerator::loadGraphs(path)[0];
    graph.printGraph();
    graph.printAdjMatrix();

    ParallelSSSP p = ParallelSSSP(2, graph, sourceVertex);
    p.solveSSSP();
    //print distance and predecessor vectors
    vector<double> dist = p.getDist();
    vector<int> pred = p.getPred();
    //print distance to destination vertex
    cout << "Distance to destination vertex: " << dist[destinationVertex] << endl;
    //print path to destination vertex

    return 0;
}



//TODO figure out why find requests is not working properly, never deletes all of the edges from bucket 0, 
// do we need to lock insert and erase from set?
// CURRENT RESULTS on ./parallel_shortest_paths basic.txt 0 2, delta = 1 (all edges are light)
// Graph 3, Vertices 3, Edges 3
// 0 1 0.5
// 1 2 0.3
// 0 2 0.9
// Adjacency Matrix:
// 0 0.5 0.9 
// 0 0 0.3 
// 0 0 0 
// Light Edges for vertex 0: 1 2 
// Light Edges for vertex 1: 2 
// Light Edges for vertex 2: 
// Heavy Edges for vertex 0: 
// Heavy Edges for vertex 1: 
// Heavy Edges for vertex 2: 
// delta: 1.000000
// number of vertices: 3
// to resize: 0
// buckets size: 4
// starting buckets:
// printing buckets[0] of size 1:
// 0 
// printing buckets[1] of size 0:

// printing buckets[2] of size 0:

// printing buckets[3] of size 2:
// 1 2 
// req_threads: 1
// erased 0 from bucket 0
// printing buckets[0] of size 0:

// erased 1 from bucket 0
// printing buckets[0] of size 0:

// performing relaxation on edge (0, 2) with weight 0.900000
// dist of 2: 3.000000
// tentative: 0.900000
// moving 2 from bucket 3 to bucket 0
// updated dist of 2: 0.900000
// performing relaxation on edge (1, 2) with weight 0.300000
// dist of 2: 0.900000
// tentative: 3.300000
// performing relaxation on edge (0, 1) with weight 0.500000
// dist of 1: 3.000000
// tentative: 0.500000
// moving 1 from bucket 3 to bucket 0
// updated dist of 1: 0.500000
// printing buckets[0] of size 2:
// 1 2 
// printing buckets[1] of size 0:

// printing buckets[2] of size 0:

// printing buckets[3] of size 0:

// request_light size: 0
// req_threads: 2
// erased 1 from bucket 0
// printing buckets[0] of size 1:
// 2 
// erased 0 from bucket 0
// erased 2 from bucket 0
// printing buckets[0] of size 0:

// printing buckets[0] of size 0:

// erased 1 from bucket 0
// printing buckets[0] of size 0:

// performing relaxation on edge (0, 2) with weight 0.900000
// dist of 2: 0.900000
// tentative: 0.900000
// performing relaxation on edge (1, 2) with weight 0.300000
// dist of 2: 0.900000
// tentative: 0.800000
// moving 2 from bucket 0 to bucket 0
// performing relaxation on edge (1, 2) with weight 0.300000
// dist of 2: 0.800000
// tentative: 0.800000
// performing relaxation on edge (0, 1) with weight 0.500000
// dist of 1: 0.500000
// tentative: 0.500000
// updated dist of 2: 0.800000
// printing buckets[0] of size 1:
// 2 
// printing buckets[1] of size 0:

// printing buckets[2] of size 0:

// printing buckets[3] of size 0:

// request_light size: 0