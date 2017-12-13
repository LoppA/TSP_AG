all:
	g++ TSP_AG.cpp -o TSP_AG.out -Wall -O3
	g++ TSP_DP.cpp -o TSP_DP.out -Wall -O3

run:
	./TSP_AG.out <in

in:
	g++ ajeita_input.cpp -o ajeita_input.out -Wall -O3
	./ajeita_input.out <data_for_tsp/distances.txt >in

test:
	./TSP_DP.out <in
