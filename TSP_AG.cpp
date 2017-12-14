#include "TSP_AG.h"
#include "draw.h"

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
	random_shuffle(pos+1, pos + N);

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

int gen = 0, tx_mut = INI_MUT, igual = 0;
double last_fit;
Gen pop[POP], best;
const double mut[11] = {0.1, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

bool raffle (double prob) {
	return rand() < ceil(prob * RAND_MAX);
}

bool vis[N];
Gen cross (Gen mama, Gen papa) {
	Gen child;
	memset (vis, 0, sizeof vis);

	int split = rand()%(N + 1);

	if (rand()%2) {
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
	} else {
		for (int i = split; i < N; i++) {
			child.pos[i] = mama.pos[i];
			vis[child.pos[i]] = true;
		}

		for (int i = 0; i < split; i++) {
			if (!vis[papa.pos[i]]) {
				child.pos[i] = papa.pos[i];
				vis[child.pos[i]] = true;
			} else {
				child.pos[i] = -1;
			}
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

	if (igual >= CHANGE_MUT) {
		igual = 0;
		if (tx_mut == MIN_MUT) 
			tx_mut = MAX_MUT;
		else 
			tx_mut--;
	}

	for (int i = 0; i < tx_mut; i++) {
		if (raffle(mut[tx_mut])) {
			int i = rand()%(N - 1) + 1, j = rand()%(N - 1) + 1;
			swap (child.pos[i], child.pos[j]);
		}
	}

	child.rate();
	return child;
}

void reproduction () {
	sort (pop, pop + POP);

	if (gen%PRED_FREC == 0) 
		predation();

	vector <Gen> parents, next_gen;

	for (int i = 0; i < POP; i++) {
		double chance = (double)(i + 1) / (double)POP;
		chance *= chance;
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
	int pred = mut[tx_mut] * PRED_RATE * POP;
	for (int i = 0; i < pred; i++)
		pop[i] = Gen();
}

int main (int argc, char *argv[]) {
	init(&argc, argv, N);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (scanf ("%lf", &dist[i][j]) != 1) printf("Reading Error\n");

	for (int i = 0; i < POP; i++)
		pop[i] = Gen();
	best = pop[0];

	FILE * fit_file = fopen(FIT_FILENAME, "w");
	last_fit = -1.0;
	while (gen++ < GEN) {
		printf ("Gen: %d\n", gen);
		for (int i = 0; i < POP; i++) {
			//printf ("%lf\n", pop[i].fitness);
			if (pop[i].fitness < best.fitness)
				best = pop[i];
		}
		fprintf(fit_file, "%lf\n", best.fitness);
		set_draw(best.pos);
		draw();

		if (last_fit == best.fitness)
			igual++;
		else
			igual = 0;
		last_fit = best.fitness;

		printf ("   Best: %lf  tx_mut: %d\n\n", best.fitness, tx_mut);
		reproduction();
	}
	fclose(fit_file);

	best.print();
	printf ("%lf\n", best.fitness);

	set_draw(best.pos);
	glutTimerFunc(0, &redraw, 0);
	glutMainLoop();
	return 0;
}
