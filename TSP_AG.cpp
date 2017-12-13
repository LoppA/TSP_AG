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

double dist[POP][POP];

Gen::Gen() {
	for (int i = 0; i < N; i++)
		pos[i] = i;
	random_shuffle(pos, pos + N);

	rate();
}

void Gen::print () {
	for (int i = 0; i < N; i++)
		cout << pos[i] << " ";
	cout << endl;
}

bool f = 0;
double Gen::rate () {
	double fit = 0;

	for (int i = 0; i < POP; i++) {
		int j = (i + 1)%POP;
		fit += dist[pos[i]][pos[j]];
	}

	if (fit == 0.0 and f) {
		print();
	
		for (int i = 0; i < POP; i++) {
			int j = (i + 1)%POP;
			printf ("%d %d: %lf\n", pos[i], pos[j], dist[pos[i]][pos[j]]);
		}

		exit(0);
	}

	return this->fitness = fit;
}

void Gen::operator = (const Gen &other) {
	memcpy (this->pos, other.pos, POP * sizeof (int));
	this->fitness = other.fitness;
}

bool Gen::operator < (const Gen &other) const {
	return this->fitness > other.fitness;
}

Gen pop[POP], best;

bool raffle (double prob) {
	return rand() < ceil(prob * RAND_MAX);
}

bool vis[POP];
Gen cross (Gen mama, Gen papa) {
	Gen child;

	memset (vis, 0, sizeof vis);
	for (int i = 0; i < POP; i++) {
		child.pos[i] = -1;
		if (!vis[mama.pos[i]] and !vis[papa.pos[i]]) {
			if (raffle(0.5))	child.pos[i] = mama.pos[i];
			else				child.pos[i] = papa.pos[i];
		} else if (!vis[mama.pos[i]]) {
			child.pos[i] = mama.pos[i];
		} else if (!vis[papa.pos[i]]) {
			child.pos[i] = papa.pos[i];
		} 

		if (child.pos[i] != -1)
			vis[child.pos[i]] = true;
	}

	vector <int> unused;
	for (int i = 0; i < POP; i++)
		if (!vis[i])
			unused.push_back(i);
	random_shuffle(unused.begin(), unused.end());

	int j = 0;
	for (int i = 0; i < POP; i++)
		if (child.pos[i] == -1)
			child.pos[i] = unused[j++];

	child.rate();
	return child;
}

void reproduction () {
	sort (pop, pop + POP);

	vector <Gen> parents, next_gen;

	for (int i = 0; i < POP; i++) {
		double chance = (double)(i + 1) / (double)POP;
		if (raffle(chance)) 
			parents.push_back(pop[i]);
	}

	next_gen.push_back(parents[parents.size() - 1]);
	while (next_gen.size() < POP) {
		Gen mama = parents[rand()%parents.size()];
		Gen papa = parents[rand()%parents.size()];

		next_gen.push_back(cross(mama, papa));
	}

	for (int i = 0; i < POP; i++)
		pop[i] = next_gen[i];
}

int main (int argc, char *argv[]) {
	srand(time(NULL));

	for (int i = 0; i < POP; i++) 
		for (int j = 0; j < POP; j++) 
			scanf ("%lf", &dist[i][j]);

	f = 1;

	for (int i = 0; i < POP; i++)
		pop[i] = Gen();

	best = pop[0];
	for (int i = 0; i < POP; i++) 
		if (pop[i].fitness < best.fitness)
			best = pop[i];

	int gen = GEN;
	while (gen--) {
		printf ("Gen: %d\n", GEN - gen);
		for (int i = 0; i < POP; i++)
			printf ("%lf\n", pop[i].fitness);
		printf ("   Best: %lf\n\n", best.fitness);
		reproduction();
	}

	best.print();
	printf ("%lf\n", best.fitness);

	return 0;
}
