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

double Gen::rate () {
	double fit = 0;

	for (int i = 0; i < POP; i++) {
		int j = (i + 1)%POP;
		fit += dist[pos[i]][pos[j]];
	}

	return this->fitness = fit;
}

void Gen::operator = (const Gen &other) {
	memcpy (this->pos, other.pos, POP * sizeof (int));
	this->fitness = other.fitness;
}

Gen pop[POP], best;

int main (int argc, char *argv[]) {
	srand(time(NULL));

	for (int i = 0; i < POP; i++) 
		for (int j = 0; j < POP; j++) 
			scanf ("%lf", &dist[i][j]);

	best = pop[0];
	for (int i = 0; i < POP; i++)
		if (pop[i].fitness > best.fitness)
			best = pop[i];

	int gen = GEN;
	while (gen--) {
	}

	return 0;
}
