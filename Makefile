# # to copy to the concole: export PATH=/usr/local/cuda/bin:$PATH


# CXX = g++
# CFLAGS = -pthread -std=c++11 -Wall

# SOURCES_GEN = graph_gen.cpp graph.cpp
# OBJECTS_GEN = graph_gen.o graph.o 
# OBJECTS_SEQ = seq_shortest_path.o graph.o

# seq_shortest_path: $(OBJECTS_SEQ)
# 	$(CXX) $(CFLAGS) -o seq_shortest_path $(OBJECTS_SEQ)

# seq_shortest_path.o: seq_shortest_path.cpp
# 	$(CXX) -c $(CFLAGS) -o seq_shortest_path.o seq_shortest_path.cpp

# graph_gen: $(OBJECTS_GEN)
# 	$(CXX) $(CFLAGS) -o graph_gen $(OBJECTS_GEN) 

# graph_gen.o: graph_gen.cpp
# 	$(CXX) -c $(CFLAGS) -o graph_gen.o graph_gen.cpp

# graph.o: graph.cpp
# 	$(CXX) -c $(CFLAGS) -o graph.o graph.cpp


# # grading.o: grading/grading.cpp gradinglib/gradinglib.hpp td6.cpp
# # 	$(CXX) -c $(CFLAGS) -o grading.o grading/grading.cpp -I.

# # main.o: main.cpp grading/grading.hpp
# # 	$(CXX) -c $(CFLAGS) -o main.o main.cpp

# clean:
# 	rm -f *.o
# 	rm -f graph_gen


# # test:
# # 	nvcc test.cu -o test -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# # graph_cuda:
# # 	nvcc graph.cu -o graph -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# # graph_seq:
# # 	gcc .cu -o sum_distances -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# # quiz:
# # 	nvcc quiz.cu -o quiz -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# # clean:
# # 	# rm -f demo
# # 	# rm -f sum_distances
# # 	# rm -f pde
# # 	# rm -f quiz


# Compiler options
CXX = g++
NVCC = nvcc
CFLAGS = -pthread -std=c++11 -Wall
NVCCFLAGS = -arch=sm_60 -std=c++11

# Source files and object files
SRC_DIR = src
GEN_SRC = $(SRC_DIR)/graph_gen.cpp $(SRC_DIR)/graph.cpp
GEN_OBJ = graph_gen.o graph.o
PAR_SRC = $(SRC_DIR)/parallel_shortest_paths.cpp $(SRC_DIR)/graph.cpp $(SRC_DIR)/graph_gen.cpp
PAR_OBJ = parallel_shortest_paths.o graph.o graph_gen.o
SEQ_SRC = $(SRC_DIR)/seq_shortest_path.cpp $(SRC_DIR)/graph.cpp $(SRC_DIR)/graph_gen.cpp
SEQ_OBJ = seq_shortest_path.o graph.o graph_gen.o

# Executables
EXE_DIR = .
GEN_EXE = $(EXE_DIR)/graph_gen
PAR_EXE = $(EXE_DIR)/parallel_shortest_paths
SEQ_EXE = $(EXE_DIR)/seq_shortest_path

# Targets
all: $(GEN_EXE) $(PAR_EXE) $(SEQ_EXE)

$(GEN_EXE): $(GEN_OBJ)
	$(CXX) $(CFLAGS) -o $(GEN_EXE) $(GEN_OBJ)

$(PAR_EXE): $(PAR_OBJ)
	$(NVCC) $(NVCCFLAGS) -o $(PAR_EXE) $(PAR_OBJ)

$(SEQ_EXE): $(SEQ_OBJ)
	$(CXX) $(CFLAGS) -o $(SEQ_EXE) $(SEQ_OBJ)

graph_gen.o: $(SRC_DIR)/graph_gen.cpp $(SRC_DIR)/graph.h
	$(CXX) -c $(CFLAGS) -o graph_gen.o $(SRC_DIR)/graph_gen.cpp

parallel_shortest_paths.o: $(SRC_DIR)/parallel_shortest_paths.cpp $(SRC_DIR)/graph.h $(SRC_DIR)/graph_gen.h 
	$(CXX) -c $(CFLAGS) -o parallel_shortest_paths.o $(SRC_DIR)/parallel_shortest_paths.cpp

seq_shortest_path.o: $(SRC_DIR)/seq_shortest_path.cpp $(SRC_DIR)/graph.h $(SRC_DIR)/graph_gen.h
	$(CXX) -c $(CFLAGS) -o seq_shortest_path.o $(SRC_DIR)/seq_shortest_path.cpp

graph.o: $(SRC_DIR)/graph.cpp $(SRC_DIR)/graph.h
	$(CXX) -c $(CFLAGS) -o graph.o $(SRC_DIR)/graph.cpp

clean:
	rm -f *.o $(GEN_EXE) $(PAR_EXE) $(SEQ_EXE)

