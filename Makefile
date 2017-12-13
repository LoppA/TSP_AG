all:
	g++ TSP_AG.cpp -o TSP_AG.out -Wall -O3
	g++ random_in.cpp -o random_in.out -Wall -O3

run:
	./TSP_AG.out <in

run_full:
	./random_in.out >in
	./TSP_AG.out <in

