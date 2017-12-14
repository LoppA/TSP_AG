#include "TSP_AG.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;

int main () {
	srand(time(NULL));

	FILE * coord_file = fopen(COORD_FILENAME, "w");
	vector<pair<double, double> > points;
	for (int i = 0; i < N; i++){
		points.push_back(make_pair(((rand() % 2000001) - 1000000)/1000000.0, ((rand() % 2000001) - 1000000)/1000000.0));
		fprintf(coord_file, "%d %lf %lf\n", i, points[i].second, points[i].first);
	}
	fclose(coord_file);

	double ax, ay, az, bx, by, bz;
	for (int i = 0; i < N; i++){
		ax = cos(points[i].second)*sin(points[i].first);
		ay = sin(points[i].second)*sin(points[i].first);
		az = cos(points[i].first);
		for (int j = 0; j < N; j++) {
			bx = cos(points[j].second)*sin(points[j].first);
			by = sin(points[j].second)*sin(points[j].first);
			bz = cos(points[j].first);
			printf ("%lf ", sqrt(((ax-bx)*(ax-bx)) + ((ay-by)*(ay-by)) + ((az-bz)*(az-bz))));
		}
		printf ("\n");
	}
	

	return 0;
}
