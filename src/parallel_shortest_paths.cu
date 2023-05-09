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
void delta_step_SSSP(Graph graph, int start = 0){
    const size_t THREADS_PER_BLOCK = 96;
    int bucket_index = 0;
    const size_t NUM_THREADS = max_degree(graph); //maxdegree to find optimal delta
    const size_t NUM_BLOCKS = (NUM_THREADS + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    //initialize distances to infinity
    double distances = new double[graph.getN()];
    for(int i=0;i<graph.getN();i++){
        distances[i] = numeric_limits<double>::max();
    }
    distances[start] = 0;
    //copy distances to tentative
    double *tentative_distances = new double[graph.getN()];
    for(int i=0;i<graph.getN();i++){
        tentative_distances[i] = distances[i];
    }
    
    //copy tentative distances to device
    double *d_tentative_distances;
    cudaMalloc(&d_tentative_distances, graph.getN() * sizeof(double));
    cudaMemcpy(d_tentative_distances, tentative_distances, graph.getN() * sizeof(double), cudaMemcpyHostToDevice);

    while(true){
        
    }

}