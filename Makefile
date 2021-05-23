all: build run_simple

run_all: run_astar run_2k_astar

run_simple:
	./main 2k_4 trivial dataset artifacts

run_2k_astar:
	./main 2k_3 trivial dataset artifacts && \
	./main 2k_3 manhattan dataset artifacts && \
	./main 2k_3 euclidian dataset artifacts && \
	./main 2k_3 chebyshev dataset artifacts && \
	./main 2k_3 diagonal dataset artifacts 

run_astar:
	./main astar trivial dataset artifacts && \
	./main astar manhattan dataset artifacts && \
	./main astar euclidian dataset artifacts && \
	./main astar chebyshev dataset artifacts && \
	./main astar diagonal dataset artifacts 


build:
	g++ -std=c++17 src/main.cpp -I src -fsanitize=address -O2 -Wshift-overflow -o main
