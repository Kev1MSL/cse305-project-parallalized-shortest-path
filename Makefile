# to copy to the concole: export PATH=/usr/local/cuda/bin:$PATH


CXX = g++
CFLAGS = -pthread -std=c++11 -Wall

SOURCES_GEN = graph_gen.cpp graph.cpp
OBJECTS_GEN = graph_gen.o graph.o 
OBJECTS_SEQ = seq_shortest_path.o graph.o

seq_shortest_path: $(OBJECTS_SEQ)
	$(CXX) $(CFLAGS) -o seq_shortest_path $(OBJECTS_SEQ)

seq_shortest_path.o: seq_shortest_path.cpp
	$(CXX) -c $(CFLAGS) -o seq_shortest_path.o seq_shortest_path.cpp

graph_gen: $(OBJECTS_GEN)
	$(CXX) $(CFLAGS) -o graph_gen $(OBJECTS_GEN) 

graph_gen.o: graph_gen.cpp
	$(CXX) -c $(CFLAGS) -o graph_gen.o graph_gen.cpp

graph.o: graph.cpp
	$(CXX) -c $(CFLAGS) -o graph.o graph.cpp


# grading.o: grading/grading.cpp gradinglib/gradinglib.hpp td6.cpp
# 	$(CXX) -c $(CFLAGS) -o grading.o grading/grading.cpp -I.

# main.o: main.cpp grading/grading.hpp
# 	$(CXX) -c $(CFLAGS) -o main.o main.cpp

clean:
	rm -f *.o
	rm -f graph_gen


# test:
# 	nvcc test.cu -o test -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# graph_cuda:
# 	nvcc graph.cu -o graph -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# graph_seq:
# 	gcc .cu -o sum_distances -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# quiz:
# 	nvcc quiz.cu -o quiz -arch=sm_60 -std=c++11 -I/usr/local/cuda/include

# clean:
# 	# rm -f demo
# 	# rm -f sum_distances
# 	# rm -f pde
# 	# rm -f quiz
