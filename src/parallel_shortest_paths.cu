#include "graph.h"
#include <algorithm>
#include <set>
#include <numeric>
#include <cmath>
using namespace std;
using Request_set = std::set<std::tuple<int, int, int>>; //define alias type for request


// __device__ void relax(int u, int v, double w, double* distances_d, double delta, int* p, set<int>* buckets){ 
//     //compute distance to v via u vs distance to v already. (this function assumes (u,v, w) not (v, u, w)))
//     double tentative = distances_d[u] + w;
//     if (distances_d[v] > tentative){
//         distances_d[v] = tentative;
//         int i = floor(distances_d[v]/delta);
//         int j = floor(tentative/delta);
//         buckets[i].erase(v);
//         buckets[j].insert(v); //move edge to new bucket 
//         p[u] = 1;

//     }
// }

// loop 3
// __global__ void relaxRequestsParallel(Request_set requests, double* distances_d, double delta, int* p, set<int>* buckets){
//     int tid = threadIdx.x + blockIdx.x * blockDim.x;
//     for (auto request : requests){
//         requests.erase(request);
//         relax(std::get<0>(request), std::get<1>(request), std::get<2>(request), distances_d, delta, p, buckets);
//     } 
// }
// 
//TODO: test max degree parallel
int max_degree(Graph graph){
    // assuming that degrees are already found.
    int max_degree = *std::max_element(graph.getGraphDegrees().begin(), graph.getGraphDegrees().end());
    return max_degree;
}


/**
 * @brief find an generate requests chunk by chunk on gpu approach
    * @param graph a graph object with N vertices and M edges
    * @param k delta bucket index
    * @param chunk_size the size of the chunk to be processed by each thread
    * @param N_light the light neighbours of each vertex, list of dynamic length vectors
    * @param N_heavy the heavy neighbours of each vertex, list of dynamic length vectors
    * @param delta the delta value
 */
// __global__ void
// findRequestsParallel(Request_set* request_heavy,
//                 Request_set* request_light,
//                 Graph* graph, 
//                 int k, 
//                 size_t chunk_size,
//                 vector* N_light,
//                 vector* N_heavy,
//                 double delta){
//     int tid = threadIdx.x + blockIdx.x * blockDim.x;
//     size_t begin = tid * chunk_size;
//     size_t end = begin + chunk_size;
//     if (end > graph.getGraphNbVertices()){
//         end = graph.getGraphNbVertices();
//     }
//     for (int v = begin; v < end; v++){
//         //GenRequests 
//         for (size_t i = 0; i < N_light[v].size(); i++){
//             request_light.insert(std::make_tuple(v, u, graph.getEdgeWeight(v, u)));
//         }
//         for (size_t i = 0; i < N_heavy[v].size(); i++){
//             request_heavy.insert(std::make_tuple(v, u, graph.getEdgeWeight(v, u)));
//         }
//     }
   
// }

__global__ void lightHeavyNeighborsParallel(double** AdjMat, size_t N, std::vector<int>* N_light, std::vector<int>* N_heavy, double delta, size_t chunk_size){
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    size_t begin = tid * chunk_size;
    size_t end = begin + chunk_size;
    if (end > N){
        end = N;
    }
    
    //use adjacency matrix to find light and heavy neighbours
    for (int v = begin; v < end; v++){
        for (int u = 0; u < N; u++){
            if (AdjMat[v][u] <= delta){
                N_light[v].push_back(u);
            }
            else{
                N_heavy[v].push_back(u);
            }
        }
    }
}
void delta_step_SSSP_Parallel(Graph graph, int start = 0){
    const size_t THREADS_PER_BLOCK = 96;
    int bucket_index = 0;
    const size_t NUM_THREADS = max_degree(graph); //maxdegree to find optimal delta
    double delta = 1/NUM_THREADS;
    size_t chunk_size = graph.getGraphNbVertices() / NUM_THREADS;
    const size_t NUM_BLOCKS = (NUM_THREADS + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

    std::vector<int>* N_light;
    std::vector<int>* N_heavy;
    cudaMalloc(&N_light, graph.getGraphNbVertices() * sizeof(vector<int>));
    cudaMalloc(&N_heavy, graph.getGraphNbVertices() * sizeof(vector<int>));
    //copy adj matrix to device
    std::vector<std::vector<double>> AdjMat = graph.getAdjMatrix();
    double** AdjMat_d; 
    for (int i = 0; i < graph.getGraphNbVertices(); i++){
        cudaMalloc(&AdjMat_d[i], graph.getGraphNbVertices() * sizeof(double));
        cudaMemcpy(AdjMat_d[i], AdjMat[i].data(), graph.getGraphNbVertices() * sizeof(double), cudaMemcpyHostToDevice);
    }

    //find light and heavy neighbours
    lightHeavyNeighborsParallel<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(AdjMat_d, graph.getGraphNbVertices(), N_light, N_heavy, delta, chunk_size);
    cudaDeviceSynchronize();

    std::vector<int>* N_light_h = (std::vector<int>*)malloc(graph.getGraphNbVertices() * sizeof(vector<int>));
    std::vector<int>* N_heavy_h = (std::vector<int>*)malloc(graph.getGraphNbVertices() * sizeof(vector<int>));
    cudaMemcpy(N_light_h, N_light, graph.getGraphNbVertices() * sizeof(vector<int>), cudaMemcpyDeviceToHost);
    cudaMemcpy(N_heavy_h, N_heavy, graph.getGraphNbVertices() * sizeof(vector<int>), cudaMemcpyDeviceToHost);
    cudaFree(N_light);
    cudaFree(N_heavy);
    //print result 
    for(int i=0;i<graph.getGraphNbVertices();i++){
        cout<<"N_light["<<i<<"] = ";
        for(int j=0;j<N_light_h[i].size();j++){
            cout<<N_light_h[i][j]<<" ";
        }
        cout<<"N_heavy["<<i<<"] = ";
        for(int j=0;j<N_heavy_h[i].size();j++){
            cout<<N_heavy_h[i][j]<<" ";
        }
       
    }
}
/**
 * @brief SSSP using delta stepping algorithm
 * @param graph a graph object with N vertices and M edges
 */
// void delta_step_SSSP_Parallel(Graph graph, int start = 0){
//     const size_t THREADS_PER_BLOCK = 96;
//     int bucket_index = 0;
//     const size_t NUM_THREADS = max_degree(graph); //maxdegree to find optimal delta
//     const size_t NUM_BLOCKS = (NUM_THREADS + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

//     //initialize distances output and also distances on device
//     // vector<double> distances(graph.getGraphNbVertices(), std::numeric_limits<double>::infinity());
//     // distances[start] = 0;
//     // double* distances_d;
//     // cudaMalloc(&distances_d, graph.getGraphNbVertices() * sizeof(double));
//     // cudaMemcpy(distances_d, distances.data(), graph.getGraphNbVertices() * sizeof(double), cudaMemcpyHostToDevice);

//     // //initialize buckets
//     // //add all vertices to bucket_inf except start
        set<int> bucket_inf;
        set::iota(std::inserter(bucket_inf, bucket_inf.begin()), 0, graph.getGraphNbVertices());
//     // bucket_inf.erase(start);
//     // //as all the weights between 0 and 1, the max path distance should be less than the number of vertices
//     // vector<set<int>> buckets[graph.getGraphNbVertices()]; 
//     // buckets[0].insert(start);

//     // int k = 0;
//     //test light and heavy neighbours parallel:
//     std::vector* N_light;
//     std::vector* N_heavy;
//     cudaMalloc(&N_light, graph.getGraphNbVertices() * sizeof(vector));
//     cudaMalloc(&N_heavy, graph.getGraphNbVertices() * sizeof(vector));
//     lightHeavyNeighborsParallel<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(graph, N_light, N_heavy, delta, chunk_size);
//     cudaDeviceSynchronize();
//     //print result 
//     for(int i=0;i<graph.getGraphNbVertices();i++){
//         cout<<"N_light["<<i<<"] = ";
//         printVector<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(N_light[i]);
//         cout<<"N_heavy["<<i<<"] = ";
//         printVector<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(N_heavy[i]);
//     }
//     // while (k < std::numeric_limits<double>::infinity()){
//     //     Request_set request_heavy, request_light; // (u, v, w) tuples
//     //     while (!buckets[k].empty()){
            
//     //     }
//     // }
// }