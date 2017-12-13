#include "TSP_AG.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <GL/glut.h>

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

int gen = 0, tx_mut = N_INI_MUT, igual = 0;
double last_fit, mut = INI_MUT;
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

	if (igual >= CHANGE_MUT) {
		igual = 0;
		if (tx_mut == N_MIN_MUT) {
			tx_mut = N_MAX_MUT;
			mut = MAX_MUT;
		} else {
			tx_mut--;
			mut -= INC_MUT;
		}
	}

	int ct = 0;
	while (ct++ < tx_mut and raffle(mut)) {
		int i = rand()%(N - 1) + 1, j = rand()%(N - 1) + 1;

		swap (child.pos[i], child.pos[j]);
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
	int pred = mut * PRED_RATE * POP;
	for (int i = 0; i < pred; i++)
		pop[i] = Gen();
}

int main (int argc, char *argv[]) {
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(WINDOW_POS, WINDOW_POS);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutCreateWindow("TSP");

	//Setar os parametros do OpenGL
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);

	for (int i = 0; i < N; i++) 
		for (int j = 0; j < N; j++) 
			scanf ("%lf", &dist[i][j]);

	for (int i = 0; i < POP; i++)
		pop[i] = Gen();
	best = pop[0];

	last_fit = -1.0;
	while (gen++ < GEN) {
		printf ("Gen: %d\n", gen);
		for (int i = 0; i < POP; i++) {
			//printf ("%lf\n", pop[i].fitness);
			if (pop[i].fitness < best.fitness)
				best = pop[i];
		}

		if (last_fit == best.fitness)
			igual++;
		else
			igual = 0;
		last_fit = best.fitness;

		printf ("   Best: %lf  mut: %lf   tx_mut: %d\n\n", best.fitness, mut, tx_mut);
		reproduction();
	}

	best.print();
	printf ("%lf\n", best.fitness);

	glutMainLoop();
	return 0;
}
