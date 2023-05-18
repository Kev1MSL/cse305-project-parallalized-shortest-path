cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
#project(cse305_project LANGUAGES CXX CUDA)
project(cse305_project)


find_package(Boost)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_STANDARD 20)


# add_library(graph_gen STATIC
# src/graph_gen.cpp
# src/graph_gen.h
# src/graph.cpp
# src/graph.h
#         )

add_executable(graph_gen test_generate_graph.cpp src/graph_gen.cpp src/graph_gen.h src/graph.cpp src/graph.h)
add_executable(seq_shortest_path src/seq_shortest_path.h src/graph_gen.h src/graph_gen.cpp src/graph.cpp src/graph.h "src/deltastep_seq.cpp" "src/deltastep_seq.h" test_seq_shortest_path.cpp)

target_link_libraries(seq_shortest_path ${Boost_LIBRARIES})

#target_compile_features(graph_gen PUBLIC cxx_std_11)
#target_compile_features(seq_shortest_path PUBLIC cxx_std_11)
#target_compile_features(graph_gen PUBLIC)
#target_compile_features(seq_shortest_path PUBLIC )


#set_target_properties( graph_gen
#        PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_ARCHITECTURES native)
#
#set_target_properties( seq_shortest_path
#        PROPERTIES CUDA_SEPARABLE_COMPILATION ON CUDA_ARCHITECTURES native)