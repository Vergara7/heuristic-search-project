all: build_anya build run_for_report

run_all: run_astar run_2k_astar run_theta run_anya

run_simple_anya: 
	./main_anya dataset artifacts

run_simple:
	./main_anya dataset artifacts
	./main theta diagonal dataset artifacts

run_for_report:
	./main_anya dataset artifacts
	./main 2k_2 euclidian dataset artifacts
	./main 2k_2 h2_3 dataset artifacts
	./main 2k_3 euclidian dataset artifacts
	./main 2k_3 h2_4 dataset artifacts
	./main 2k_4 euclidian dataset artifacts
	./main 2k_4 h2_5 dataset artifacts
	./main 2k_5 euclidian dataset artifacts
	./main 2k_5 h2_6 dataset artifacts
	./main theta euclidian dataset artifacts

run_astar:
	./main astar trivial dataset artifacts && \
	./main astar manhattan dataset artifacts && \
	./main astar euclidian dataset artifacts && \
	./main astar chebyshev dataset artifacts && \
	./main astar diagonal dataset artifacts

run_2k_astar:
	./main 2k_3 trivial dataset artifacts && \
	./main 2k_3 manhattan dataset artifacts && \
	./main 2k_3 euclidian dataset artifacts && \
	./main 2k_3 chebyshev dataset artifacts && \
	./main 2k_3 diagonal dataset artifacts 

run_theta:
	./main theta trivial dataset artifacts && \
	./main theta manhattan dataset artifacts && \
	./main theta euclidian dataset artifacts && \
	./main theta chebyshev dataset artifacts && \
	./main theta diagonal dataset artifacts

run_anya:
	./main_anya dataset artifacts

build:
	g++ -std=c++17 src/main.cpp -I src -fsanitize=address -O2 -Wshift-overflow -o main

build_anya:
	g++ -std=c++17 src/main_anya.cpp src/algo/anya.cpp -I src -fsanitize=address -O2 -Wshift-overflow -o main_anya
