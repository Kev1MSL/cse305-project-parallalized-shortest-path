# CSE305 Project - Parallalized Shortest Path

This is a CMake project for the CSE305 project. It includes code for generating graphs and performing shortest path computations using sequential and parallel algorithms.

## Prerequisites

- CMake (version 3.8 or higher)
- C++ compiler with C++20 support
- Boost library

## Getting Started

1. Clone the repository:

```shell
git clone https://github.com/your-username/cse305_project.git
cd cse305_project
```

2. Build the project:

```
cmake .
make
```

3. Run the executables:

*Disclaimer: Even if we ask for a target_vertex, both sequential and parallel version computes the distances to all vertices. This could of course be fixed, but we wanted to check that our implementation is correct and thread sage (no race conditions, segfaults, etc.)*

**graph_gen:** Generates a graph.

```
./graph_gen <number of vertices> <number of edges> <number of graphs> <filename>
```

**seq_shortest_path:** Performs shortest path computation using the sequential algorithm.

```
./seq_shortest_path <graph_filename> <from_vertex> <to_vertex>
```

**parallel_shortest_path:** Performs shortest path computation using the parallel algorithm.

```
./parallel_shortest_path <graph_filename> <from_vertex> <to_vertex> <threads_desired>
```

4. Example
Usage

```
./parallel_shortest_path graph.txt 0 2 8
```


## Project Structure

The project has the following structure:

**src/:** Contains the source code files.

**test_generate_graph.cpp:** Test file for graph generation.

**test_seq_shortest_path.cpp:** Test file for sequential shortest path computation.

**test_parallel_shortest_path.cpp:** Test file for parallel shortest path computation.

## Graphs

We provides some graphs to run the experiments on:

- basic.txt
- test.txt
- 100v_2ke.txt (100 vertices, 2k edges)
- 1kv_20ke.txt (1k vertices, 20k edges)
- 1kv_100ke.txt (1k vertices, 100k edges)
- graph.txt (1k vertices, 300k edges)

