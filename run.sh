#!/bin/bash 
make clean
make graph_gen
./graph_gen
# make seq_shortest_path
# ./seq_shortest_path test.txt 0 5