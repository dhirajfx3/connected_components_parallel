# connected_components_parallel
## Compilation
### g++ -std=c++11 -O3 dfs.cpp
### gcc -O3 dsu.c 
### g++ -std=c++11 -O3 verify.cpp
### nvcc -Xcompiler /openmp -O3 cc.cu
## Input format : N M // Number of nodes and number of edges
### M lines containing edges (u,v)
### See sample.txt
### using verifier : 
#### $./verify output_file1.txt output_file2.txt
#### $Results do not match!
