all:
	g++ draw.cpp TSP_AG.cpp -o TSP_AG.out -Wall -Wextra -Wno-unused-parameter -O3 -lGL -lglut -lm
	g++ draw.cpp TSP_DP.cpp -o TSP_DP.out -Wall -Wextra -Wno-unused-parameter -O3 -lGL -lglut -lm
	g++ draw.cpp TSP_TRIVIAL.cpp -o TSP_TRIVIAL.out -Wall -Wextra -Wno-unused-parameter -O3 -lGL -lglut -lm
	g++ random_in.cpp -o random_in.out -Wall -Wextra -O3 -lm

maps_case:
	python ./data_for_tsp/coord_getter.py > ./data_for_tsp/coordinates.txt
	python ./data_for_tsp/distance_getter.py > ./data_for_tsp/distances.txt

random_case:
	./random_in.out > ./data_for_tsp/random_distances.txt

run:
	./TSP_AG.out < ./data_for_tsp/distances.txt
	./TSP_TRIVIAL.out < ./data_for_tsp/distances.txt

random_run:
	./TSP_AG.out < ./data_for_tsp/random_distances.txt
	./TSP_TRIVIAL.out < ./data_for_tsp/random_distances.txt

test:
	./TSP_DP.out < ./data_for_tsp/distances.txt

random_test:
	./TSP_DP.out < ./data_for_tsp/random_distances.txt

graphic:
	python ./plot/plot.py

map:
	python ./generate_map/generate_map.py > generate_map/path_map.html

clean:
	rm -f *.out ./data_for_tsp/*coordinates* ./data_for_tsp/*distances* ./plot/graphic_best.png
