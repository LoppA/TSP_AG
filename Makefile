all:
	g++ TSP_AG.cpp -o TSP_AG.out -Wall -O3 -lGL -lglut
	g++ TSP_DP.cpp -o TSP_DP.out -Wall -O3 -lGL -lglut

maps_case:
	python ./data_for_tsp/coord_getter.py > ./data_for_tsp/coordinates.txt
	python ./data_for_tsp/distance_getter.py > ./data_for_tsp/distances.txt
	cp ./data_for_tsp/distances.txt ./in

random_case:
	g++ random_in.cpp -o random_in.out -Wall -O3 -lGL -lglut
	./random_in.out > in

run:
	./TSP_AG.out < in

test:
	./TSP_DP.out < in 
