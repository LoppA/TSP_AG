#include "TSP_AG.h"
#include <iostream>
#include <cfloat>
#include <cstring>

using namespace std;

const int M = 20;
bool vis[M][(1<<M) + 100];
double dp[M][(1<<M) + 100], dist[N][N];

double go (int at, int mask) {

	double &r = dp[at][mask];

	if (vis[at][mask]) 
		return r;
	vis[at][mask] = 1;
	r = DBL_MAX;

	bool f = false;
	for (int i = 0, msk = 1; i < N; i++, msk <<= 1) 
		if (!(msk & mask)) {
			r = min (r, go (i, mask | msk) + dist[at][i]);
			f = true;
		}

	if (!f) {
		r = dist[at][0];
	}

	return r;
}

int main (void) {
	memset (vis, 0, sizeof vis);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> dist[i][j];

	printf ("%lf\n", go (0, 1));

	return 0;
}
