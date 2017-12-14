#include "TSP_AG.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <vector>
#include <map>
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

Point3D::Point3D() : Point3D(0.0, 0.0, 0.0){}
Point3D::Point3D(GLdouble x, GLdouble y, GLdouble z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Polar3D Point3D::to_Polar3D(){
	GLdouble aux_p, aux_the, aux_phi;
	aux_p = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	aux_the = atan(this->y/this->x);
	aux_phi = acos(this->z/aux_p);
	return Polar3D(aux_p, aux_the, aux_phi);
}

Polar3D::Polar3D() : Polar3D(0.0, 0.0, 0.0){}
Polar3D::Polar3D(GLdouble p, GLdouble the, GLdouble phi){
	this->p = p;
	this->the = the;
	this->phi = phi;
}
Point3D Polar3D::to_Point3D(){
	GLdouble aux_x, aux_y, aux_z;
	aux_x = this->p*cos(this->the)*sin(this->phi);
	aux_y = this->p*sin(this->the)*sin(this->phi);
	aux_z = this->p*cos(this->phi);
	return Point3D(aux_x, aux_y, aux_z);
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

void apply_matrix(GLdouble * mat, Point3D * vect_in, Point3D * vect_out){
	vect_out->x = (mat[0] * vect_in->x) + (mat[4] * vect_in->y) + (mat[8] * vect_in->z) + mat[12];
	vect_out->y = (mat[1] * vect_in->x) + (mat[5] * vect_in->y) + (mat[9] * vect_in->z) + mat[13];
	vect_out->z = (mat[2] * vect_in->x) + (mat[6] * vect_in->y) + (mat[10] * vect_in->z) + mat[14];
}

void draw(map<int, Polar3D> map_coord, int * way){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLdouble mat[16];
	Point3D axis(map_coord[way[0]].to_Point3D().y, -map_coord[way[0]].to_Point3D().x, 0.0);
	double ang = (map_coord[way[0]].phi)*180.0/M_PI;
	glPushMatrix();
	glTranslated(0.0, 0.0, -1.0);
	glRotated(ang, axis.x, axis.y, axis.z);
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();
	
	Point3D in, out;
	GLdouble max_dim = DBL_MIN;
	for(int i=0; i<N; i++){
		in = map_coord[way[i]].to_Point3D();
		apply_matrix(mat, &in, &out);
		if (abs(out.x) > max_dim) max_dim = abs(out.x);
		if (abs(out.y) > max_dim) max_dim = abs(out.y);
		if (abs(out.z) > max_dim) max_dim = abs(out.z);
	}

	glPushMatrix();
	glScaled(NORMALIZATION_VAL/max_dim, NORMALIZATION_VAL/max_dim, NORMALIZE_Z ? NORMALIZATION_VAL/max_dim : 1.0);
	glMultMatrixd(mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();
	
	/*
	//Point3D up(1.0, 0.0, 0.0);
	//apply_matrix(mat, &up, &out);
	//Polar3D up_polar = out.to_Polar3D();
	ang = -60.0;
	axis = Point3D(0.0, 0.0, 1.0);
	glPushMatrix();
	glRotated(ang, axis.x, axis.y, axis.z);
	glMultMatrixd(mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();
	*/

	vector<Point3D> points;
	for (int i=0; i<N; i++){
		in = map_coord[way[i]].to_Point3D();
		apply_matrix(mat, &in, &out);
		points.push_back(out);
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslated(points[0].x, points[0].y, points[0].z);
	glutSolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i=1; i<N; i++){
		glPushMatrix();
		glTranslated(points[i].x, points[i].y, points[i].z);
		glutSolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
		glPopMatrix();
	}

	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<N; i++) glVertex3d(points[i].x, points[i].y, points[i].z);
	glVertex3d(points[0].x, points[0].y, points[0].z);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

map<int, Polar3D> get_map_coord(){
	int id;
	GLdouble lat, lon;
	map<int, Polar3D> map_coord;
	FILE * coord_file = fopen(COORD_FILENAME, "r");
	for (int i=0; i<N; i++){
		fscanf(coord_file, "%d%lf%lf", &id, &lat, &lon);
		map_coord[id] = Polar3D(1.0, (lon+180.0)*M_PI/180.0, (lat+90.0)*M_PI/180.0);
	}
	fclose(coord_file);

	return map_coord;
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

	map<int, Polar3D> map_coord = get_map_coord();
	draw(map_coord, best.pos);
	glutMainLoop();
	return 0;
}
