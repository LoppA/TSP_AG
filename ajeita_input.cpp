#include "TSP_AG.h"
#include <iostream>
#include <cstdio>

using namespace std;

double dist[N][N];

int main (void) {

	int a, b;
	double d;

	string lixo;
	getline (cin, lixo);

	while (cin >> a >> b >> d) 
		dist[a][b] = dist[b][a] = d;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) 
			printf ("%lf ", dist[i][j]);
		printf ("\n");
	}

	return 0;
}
