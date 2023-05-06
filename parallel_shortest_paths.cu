#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include "graph.cpp"


using namespace std;

void print(vector<int> &v){
    for(int i=0;i<v.size();i++){
        cout<<v[i]<<" ";
    }
    cout<<endl;
}
__global__ void delta_step_SSSP(Graph graph){
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if(tid < graph.num_vertices){
        int u = tid;
        for(int i=graph.vertices[u];i<graph.vertices[u+1];i++){
            int v = graph.edges[i];
            int w = graph.weights[i];
            if(graph.distances[v] > graph.distances[u] + w){
                graph.distances[v] = graph.distances[u] + w;
            }
        }
    }
}

__global__ 
void vertex_out_degree(Graph graph){
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    for (int i = 0; i < graph.edges; i++)
    {
        if (tid == graph.edges[i][0])
        {
            graph.degree[tid]++;
        }
    }
}
void degrees_parallel(Graph graph){
    const size_t THREADS_PER_BLOCK = 256;
    cudaMallocManaged(&graph.degree, vertices * sizeof(int));
    for (int i = 0; i < vertices; i++)
    {
        graph.degree[i] = 0;
    }
    const size_t block_size = (vertices + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    vertex_out_degree<<<block_size, THREADS_PER_BLOCK>>>(graph);
    cudaDeviceSynchronize();
    vector<int> degrees;
    for (int i = 0; i < vertices; i++)
    {
        degrees.push_back(graph.degree[i]);
    }
    //write to graph degrees
    graph.setDegrees(degrees);
}

int max_degree_parallel(Graph graph){
    graph.degrees_parallel();
    int *max_degree = max_element(graph.degree, graph.degree + graph.getN());
    return *max_degree;
}

void delta_step_SSSP(Graph graph){
    int max_degree = max_degree_parallel(graph);
    
}