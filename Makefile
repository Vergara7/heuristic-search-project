all: build run_astar

run_astar:
	./main astar trivial dataset artifacts && \
	./main astar manhattan dataset artifacts && \
	./main astar euclidian dataset artifacts && \
	./main astar chebyshev dataset artifacts && \
	./main astar diagonal dataset artifacts 

build:
	g++ -std=c++17 src/main.cpp -I src -fsanitize=address -O2 -Wshift-overflow -o main
