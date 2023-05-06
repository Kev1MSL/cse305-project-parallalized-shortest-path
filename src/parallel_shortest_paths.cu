#include graph.h

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

// 
//TODO: test max degree parallel
int max_degree(Graph graph){
    // assuming that degrees are already found.

    int *max_degree = max_element(graph.degree, graph.degree + graph.getN());
    return *max_degree;
}

/**
 * @brief SSSP using delta stepping algorithm
 * @param graph a graph object with N vertices and M edges
 */
void delta_step_SSSP(Graph graph){
    int max_degree = max_degree(graph); //maxdegree to find optimal delta
}