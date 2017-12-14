all:
	g++ TSP_AG.cpp -o TSP_AG.out -Wall -Wextra -O3 -lGL -lglut -lm
	g++ TSP_DP.cpp -o TSP_DP.out -Wall -Wextra -O3 -lGL -lglut -lm

maps_case:
	python ./data_for_tsp/coord_getter.py > ./data_for_tsp/coordinates.txt
	python ./data_for_tsp/distance_getter.py > ./data_for_tsp/distances.txt

random_case:
	g++ random_in.cpp -o random_in.out -Wall -Wextra -O3
	./random_in.out > ./data_for_tsp/random_coordinates.txt

run:
	./TSP_AG.out < ./data_for_tsp/distances.txt

test:
	./TSP_DP.out < ./data_for_tsp/distances.txt

clean:
	rm -f *.out ./data_for_tsp/coordinates.txt ./data_for_tsp/distances.txt ./data_for_tsp/random_coordinates.txt