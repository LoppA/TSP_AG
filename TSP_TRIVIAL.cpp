#include "TSP_AG.h"
#include <iostream>
#include <cfloat>
#include <cstring>
#include <queue>

using namespace std;

bool vis[N];
double dist[N][N], res2 = 0.0;
int pos[N];

void go (int i, int at) {
	priority_queue <pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;
	vis[at] = true;

	for (int i = 0; i < N; i++)
		pq.push(make_pair(dist[at][i], i));

	pos[i] = at;

	while (pq.size()) {
		double d = pq.top().first;
		int next = pq.top().second;
		pq.pop();

		if (!vis[next]) {
			res2 += d;
			go (i+1, next);
			return;
		}
	}

	res2 += dist[at][0];
}

int main (void) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> dist[i][j];

	go (0, 0);

	double res = 0;
	for (int i = 0; i < N; i++) {
		int j = (i + 1)%N;
		cout << pos[i] << " ";
		res += dist[pos[i]][pos[j]];
	}
	cout << endl;

	printf ("%lf\n", res);
	printf ("%lf\n", res2);

	return 0;
}
