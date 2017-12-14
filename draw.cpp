#include "TSP_AG.h"
#include "draw.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cfloat>
#include <vector>
#include <map>

using namespace std;

map<int, Polar3D> map_coord;
int * way;
int n;
double rot_angle, scale_x, scale_y;

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

void init(int * argc, char ** argv, int size){
	srand(time(NULL));
	way = NULL;
	n = size;
	rot_angle = 0.0;
	scale_x = scale_y = 1.0;

	int id;
	GLdouble lat, lon;
	FILE * coord_file = fopen(COORD_FILENAME, "r");
	for (int i=0; i<n; i++){
		if (fscanf(coord_file, "%d%lf%lf", &id, &lat, &lon) != 3) printf("Reading Error\n");
		map_coord[id] = Polar3D(1.0, (lon+180.0)*M_PI/180.0, (lat+90.0)*M_PI/180.0);
	}
	fclose(coord_file);

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(WINDOW_POS, WINDOW_POS);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutCreateWindow("TSP");

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glutSpecialFunc(&special_down_call);
	glutKeyboardFunc(&keyboard_down_call);
	glutDisplayFunc(&draw);
	glutTimerFunc(0, &redraw, 0);
}

void set_draw(int * new_way){
	way = new_way;
}

void special_down_call(int key, int x, int y){
	if (key == GLUT_KEY_LEFT) rot_angle += ANGLE_STEP*scale_x*scale_y;
	else if (key == GLUT_KEY_RIGHT) rot_angle -= ANGLE_STEP*scale_x*scale_y;
}

void keyboard_down_call(unsigned char key, int x, int y){
	if (key == 'h') scale_x *= -1;
	else if (key == 'v') scale_y *= -1;;
}

void apply_matrix(GLdouble * mat, Point3D * vect_in, Point3D * vect_out){
	vect_out->x = (mat[0] * vect_in->x) + (mat[4] * vect_in->y) + (mat[8] * vect_in->z) + mat[12];
	vect_out->y = (mat[1] * vect_in->x) + (mat[5] * vect_in->y) + (mat[9] * vect_in->z) + mat[13];
	vect_out->z = (mat[2] * vect_in->x) + (mat[6] * vect_in->y) + (mat[10] * vect_in->z) + mat[14];
}

void draw(){
	if (way == NULL) return;

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
	for(int i=0; i<n; i++){
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
	
	glPushMatrix();
	glScaled(scale_x, scale_y, 1.0);
	glRotated(rot_angle, 0.0, 0.0, 1.0);
	glMultMatrixd(mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();

	vector<Point3D> points;
	for (int i=0; i<n; i++){
		in = map_coord[way[i]].to_Point3D();
		apply_matrix(mat, &in, &out);
		points.push_back(out);
	}

	for (int i=0; i<n; i++){
		glColor3f(1.0f, ((double)i)*(1.0/(n-1)), ((double)i)*(1.0/(n-1)));
		glPushMatrix();
		glTranslated(points[i].x, points[i].y, points[i].z);
		glutSolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
		glPopMatrix();
	}

	glBegin(GL_LINE_STRIP);
	for (int i=0; i<n; i++){
		glColor3f(1.0f, ((double)i)*(1.0/(n-1)), ((double)i)*(1.0/(n-1)));
		glVertex3d(points[i].x, points[i].y, points[i].z);
	}
	glVertex3d(points[0].x, points[0].y, points[0].z);
	glEnd();

	glutSwapBuffers();
	glFlush();
}

void redraw(int value){
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, &redraw, 0);
}