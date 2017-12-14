#include "TSP_AG.h"
#include <iostream>
#include <cfloat>
#include <cstring>
#include <queue>

using namespace std;

bool vis[N];
double dist[N][N];

void go (int at, double res) {
	priority_queue <pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;
	vis[at] = true;

	for (int i = 0; i < N; i++)
		pq.push(make_pair(dist[at][i], i));

	cout << at << " ";

	while (pq.size()) {
		double d = pq.top().first;
		int next = pq.top().second;
		pq.pop();

		if (!vis[next]) {
			go (next, res + d);
			return;
		}
	}

	res += dist[at][0];

	printf ("\nRes: %lf\n", res);
}

int main (void) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> dist[i][j];

	go (0, 0.0);

	return 0;
}
