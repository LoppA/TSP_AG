#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#include <GL/glut.h>
#include <map>

#define N 30
#define POP 10000
#define GEN 100

#define PRED_RATE 0.3
#define PRED_FREC 10

#define CHANGE_MUT 3
#define INI_MUT 0.5
#define MAX_MUT 0.9
#define INC_MUT 0.1
#define N_INI_MUT 5
#define N_MIN_MUT 1
#define N_MAX_MUT 9

#define WINDOW_POS 20
#define WINDOW_SIZE 600

#define COORD_FILENAME "./data_for_tsp/coordinates.txt"
#define FIT_FILENAME "./fitnesses.txt"

#define SPHERE_RADIUS 0.01
#define SPHERE_SLICES 20
#define SPHERE_STACKS 20

#define NORMALIZATION_VAL 0.9
#define NORMALIZE_Z false

#define ANGLE_STEP 5.0
#define FPS 60

struct Gen {
	int pos[N];
	double fitness;

	Gen ();

	void print ();

	double rate ();

	void operator = (const Gen &other);

	bool operator < (const Gen &other) const;
};

class Point3D;
class Polar3D;

class Point3D
{
public:
	GLdouble x, y, z;

	Point3D();
	Point3D(GLdouble, GLdouble, GLdouble);
	Polar3D to_Polar3D();
};

class Polar3D
{
public:
	GLdouble p, the, phi;

	Polar3D();
	Polar3D(GLdouble, GLdouble, GLdouble);
	Point3D to_Point3D();
};

bool raffle (double);
Gen cross (Gen, Gen);
void reproduction();
void predation();

void special_down_call(int, int, int);
void keyboard_down_call(unsigned char, int, int);

void apply_matrix(GLdouble *, Point3D *, Point3D *);
void draw();
void redraw();

std::map<int, Polar3D> get_map_coord();

#endif
