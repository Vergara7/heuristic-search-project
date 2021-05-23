all: build run_astar

run_astar:
	./main astar dataset artifacts

build:
	g++ -std=c++17 src/main.cpp -I src -fsanitize=address -O2 -Wshift-overflow -o main
