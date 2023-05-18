cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cse305_project LANGUAGES CXX CUDA)


# add_library(graph_gen STATIC
# src/graph_gen.cpp
# src/graph_gen.h
# src/graph.cpp
# src/graph.h
#         )

add_executable(graph_gen test_generate_graph.cpp src/graph_gen.cpp src/graph_gen.h src/graph.cpp src/graph.h)
add_executable(seq_shortest_path src/seq_shortest_path.cpp src/graph_gen.h src/graph_gen.cpp src/graph.cpp src/graph.h "src/deltastep_seq.cpp" "src/deltastep_seq.h" )

# Request that particles be built with -std=c++11
# As this is a public compile feature anything that links to 
# particles will also build with -std=c++11
target_compile_features(graph_gen PUBLIC cxx_std_11)
target_compile_features(seq_shortest_path PUBLIC cxx_std_11)

# We need to explicitly state that we need all CUDA files in the 
# particle library to be built with -dc as the member functions
# could be called by other libraries and executables
set_target_properties( graph_gen
        PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_ARCHITECTURES native)

set_target_properties( seq_shortest_path
        PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_ARCHITECTURES native)