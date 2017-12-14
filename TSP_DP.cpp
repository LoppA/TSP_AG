#include "TSP_AG.h"
#include "draw.h"
#include <iostream>
#include <cfloat>
#include <cstring>

using namespace std;

const int M = 20;
bool vis[M][(1<<M) + 100];
double dp[M][(1<<M) + 100], dist[N][N];
int pos[N];

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

void build_pos (int at, int mask, int i_pos) {
	double &r = dp[at][mask];

	pos[i_pos] = at;

	for (int i = 0, msk = 1; i < N; i++, msk <<= 1) 
		if (!(msk & mask)) 
			if (r == go (i, mask | msk) + dist[at][i]) {
				build_pos (i, mask | msk, i_pos + 1);
				return;
			}

}

int main (int argc, char *argv[]) {
	init(&argc, argv, N);
	memset (vis, 0, sizeof vis);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> dist[i][j];

	printf ("%lf\n", go (0, 1));

	build_pos (0, 1, 0);

	for (int i = 0; i < N; i++)
		cout << pos[i] << " ";
	cout << endl;

	set_draw(pos);
	draw();

	glutMainLoop();

	return 0;
}
