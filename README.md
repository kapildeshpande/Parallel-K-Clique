# Parallel-K-Clique
Contain Parallel implementation of listing K-Clique problem.\
It implementes the algorithm discussed in the following paper\
Maximilien Danisch, Oana Balalau, and Mauro Sozio. 2018. Listing k-cliques in Sparse Real-World 
Graphs (https://oanabalalau.com/pdf/kcliques.pdf)
\
\
To compile :- g++ parallelKList.cpp -O9 -o parallelKList -fopenmp\
To run:- ./parallelKList inputfilename \
Note: if inputfile is not specified then the default edgelist.txt file is taken as an input.\
\
Input Format\
List of Edges\
Value of K
