#include "TSP_AG.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>

using namespace std;

double dist[POP][POP];

// O(N**2)
Gen::Gen() {
	for (int i = 0; i < N; i++)
		pos[i] = i;
	random_shuffle(pos, pos + N);
}

void Gen::print () {
	for (int i = 0; i < N; i++)
		cout << pos[i] << " ";
	cout << endl;
}

Gen pop[POP];

int main (int argc, char *argv[]) {
	srand(time(NULL));

	for (int i = 0; i < POP; i++) {
		for (int j = 0; j < POP; j++) {
			scanf ("%lf", &dist[i][j]);
			printf ("%lf ", dist[i][j]);
		}
		printf ("\n");
	}

	return 0;
}
