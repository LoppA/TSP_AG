#include "TSP_AG.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

double dist[N][N];

Gen::Gen() {
	for (int i = 0; i < N; i++)
		pos[i] = i;
	random_shuffle(pos + 1, pos + N);

	rate();
}

void Gen::print () {
	for (int i = 0; i < N; i++)
		cout << pos[i] << " ";
	cout << endl;
}

double Gen::rate () {
	double fit = 0;

	for (int i = 0; i < N; i++) {
		int j = (i + 1)%N;
		fit += dist[pos[i]][pos[j]];
	}

	return this->fitness = fit;
}

void Gen::operator = (const Gen &other) {
	memcpy (this->pos, other.pos, N * sizeof (int));
	this->fitness = other.fitness;
}

bool Gen::operator < (const Gen &other) const {
	return this->fitness > other.fitness;
}

int gen = 0, last_pred = 0;
Gen pop[POP], best;

bool raffle (double prob) {
	return rand() < ceil(prob * RAND_MAX);
}

bool vis[N];
Gen cross (Gen mama, Gen papa) {
	Gen child;
	memset (vis, 0, sizeof vis);

	int split = rand()%(N + 1);

	for (int i = 0; i < split; i++) {
		child.pos[i] = mama.pos[i];
		vis[child.pos[i]] = true;
	}

	for (int i = split; i < N; i++) {
		if (!vis[papa.pos[i]]) {
			child.pos[i] = papa.pos[i];
			vis[child.pos[i]] = true;
		} else {
			child.pos[i] = -1;
		}
	}

	vector <int> unused;
	for (int i = 0; i < N; i++)
		if (!vis[i])
			unused.push_back(i);
	random_shuffle(unused.begin(), unused.end());

	int j = 0;
	for (int i = 0; i < N; i++)
		if (child.pos[i] == -1)
			child.pos[i] = unused[j++];

	int ct = 0;
	while (ct++ < MX_MUT and raffle(MUT)) {
		int i = rand()%(N - 1) + 1, j = rand()%(N - 1) + 1;

		swap (child.pos[i], child.pos[j]);
	}

	child.rate();
	return child;
}

void reproduction () {
	sort (pop, pop + POP);

	last_pred++;
	if (gen%PRED_FREC) {
		predation();
		last_pred = 0;
	} else if (last_pred == ART_FREC) {
		artificial();
	}

	vector <Gen> parents, next_gen;

	for (int i = 0; i < POP; i++) {
		double chance = (double)(i + 1) / (double)POP;
		if (raffle(chance)) 
			parents.push_back(pop[i]);
	}

	next_gen.push_back(pop[POP - 1]);
	while (next_gen.size() < POP) {
		Gen mama = parents[rand()%parents.size()];
		Gen papa = parents[rand()%parents.size()];

		next_gen.push_back(cross(mama, papa));
	}

	for (int i = 0; i < POP; i++)
		pop[i] = next_gen[i];
}

void predation () {
	int pred = PRED_RATE * POP;
	for (int i = 0; i < pred; i++)
		pop[i] = Gen();
}

void artificial () {

}

int main (int argc, char *argv[]) {
	srand(time(NULL));

	for (int i = 0; i < N; i++) 
		for (int j = 0; j < N; j++) 
			scanf ("%lf", &dist[i][j]);

	for (int i = 0; i < POP; i++)
		pop[i] = Gen();

	best = pop[0];
	while (gen++ < GEN) {
		printf ("Gen: %d\n", gen);
		for (int i = 0; i < POP; i++) {
//			printf ("%lf\n", pop[i].fitness);
			if (pop[i].fitness < best.fitness)
				best = pop[i];
		}
		printf ("   Best: %lf\n\n", best.fitness);
		reproduction();
	}

	best.print();
	printf ("%lf\n", best.fitness);

	return 0;
}
