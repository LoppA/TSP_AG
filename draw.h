#ifndef __DRAW_H__
#define __DRAW_H__

#include <GL/glut.h>

#define WINDOW_POS 20
#define WINDOW_SIZE 600

#define SPHERE_RADIUS 0.01
#define SPHERE_SLICES 20
#define SPHERE_STACKS 20

#define NORMALIZATION_VAL 0.9
#define NORMALIZE_Z false

#define ANGLE_STEP 5.0
#define FPS 60

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

void init(int *, char **, int);
void set_draw(int *);

void special_down_call(int, int, int);
void keyboard_down_call(unsigned char, int, int);

void apply_matrix(GLdouble *, Point3D *, Point3D *);
void draw();
void redraw(int);

#endif