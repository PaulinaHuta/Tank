/*
Paulina Huta*/
#include "glut.h"
#include <math.h>
#include <iostream>
using namespace std;

const double PI = 4 * atan(1.0);
const int H = 600;
const int W = 600;

double dx = 0, dy = 0;
double sx = 1, sy = 1;
double angle = 0;
double direction = 1;
double beta;
double beta2, x2, y2, direction2;
double radius = .06;

double max_exploasion = 0.1;
int is_exploading;
double exploasion_x;
double exploasion_y;
double exploasion_timer;

bool is_moving = false;
double bx0 = 0, by0 = 0, vx, vy, a = -1, t = 0;
double bx = bx0, by = by0;

void init()
{
	glClearColor(0, 0, 1, 0);
	glOrtho(-1, 1, -1, 1, -1, 1);
}


void DrawWheel()
{
	double alpha, teta = PI / 20;
	double x, y;

	glBegin(GL_LINE_LOOP);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
	}
	glEnd();

	teta = PI / 4;
	glBegin(GL_LINES);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
		glVertex2d(0, 0);
	}
	glEnd();

}

void DrawCannonBall(double radius)
{
	double x, y;
	glPushMatrix();
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);

	double alpha, teta = PI / 20;
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
	}
	glEnd();

	glPopMatrix();
}

void draw_exploasion()
{
	if (is_exploading) {
		exploasion_timer += 0.003;
		if (exploasion_timer >= max_exploasion) {
			is_exploading = 0;
		}

		double x, y;
		glPushMatrix();
		glTranslated(bx, by, 0);
		glScaled(max_exploasion, max_exploasion, 1);
		x = 0.01;
		y = 0.01;
		glColor3d(1, 0.9, 0);
		glBegin(GL_POLYGON);

		int i;
		for (i = 0; i < 8; i++)
		{
			x = cos(i / 8.0 * 2 * PI);
			y = sin(i / 8.0 * 2 * PI);
			glVertex2d(x, y);
			x = 0.1 * cos(i / 8.0 * 2 * PI);
			y = 0.1 * sin(i / 8.0 * 2 * PI);
			glVertex2d(x, y);
		}
		glEnd();
		glPopMatrix();
	}
}

void myRotated(double x, double y, double teta)
{
	double prev_x = x, prev_y = y;
	x = cos(teta) * prev_x - sin(teta) * prev_y;
	y = sin(teta) * prev_x + cos(teta) * prev_y;
}

void DrawTank(double x, double y)
{
	glPushMatrix();
	glTranslated(x, y + 0.03, 0);
	glRotated(beta * 180 / PI, 0, 0, 1);
	glScaled(0.3, 0.3, 1);
	glTranslated(-0.15, 0, 0);

	glPushMatrix();
	glScaled(radius, radius, 1);
	glRotated(angle, 0, 0, direction);
	DrawWheel();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2 * radius, 0, 0);
	glScaled(radius, radius, 1);
	glRotated(angle, 0, 0, direction);
	DrawWheel();
	glPopMatrix();

	glPushMatrix();
	glTranslated(4 * radius, 0, 0);
	glScaled(radius, radius, 1);
	glRotated(angle, 0, 0, direction);
	DrawWheel();
	glPopMatrix();

	glPushMatrix();
	glTranslated(6 * radius, 0, 0);
	glScaled(radius, radius, 1);
	glRotated(angle, 0, 0, direction);
	DrawWheel();
	glPopMatrix();

	glLineWidth(2);
	glColor3d(0.6, 0.1, 0.1);
	glBegin(GL_LINES);
	glVertex2d(0, 0);
	glVertex2d(radius * 6, 0);  
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex2d(-1.7 * radius, radius);
	glVertex2d(-radius + 0.01, -radius);
	glVertex2d(7 * radius - 0.01, -radius);
	glVertex2d(1.1 * radius + 7 * radius, radius);
	glEnd();

	double hiegth_fuctor = 2.5; 
	glBegin(GL_POLYGON);
	glVertex2d(0, radius);
	glVertex2d(radius, hiegth_fuctor * radius);
	glVertex2d(5 * radius, hiegth_fuctor * radius);
	glVertex2d(6 * radius, radius);
	glEnd();

	double a = hiegth_fuctor - 1;
	double o_a = -(1 / a);
	glBegin(GL_POLYGON);
	glVertex2d(1 * radius, 2.5 * radius);
	glVertex2d(0.8 * radius, 2 * radius);
	glVertex2d(-2 * radius, 2 * radius);
	glVertex2d(-2 * radius, 2.5 * radius);
	glEnd();

	glPushMatrix();
	glTranslated(-2 * radius - 0.01, 2 * radius + 0.01, 0);
	glScaled(0.02, 0.02, 1);
	glPopMatrix();

	glLineWidth(1);
	glPopMatrix();

	glPushMatrix();
	if (is_moving)
	{
		glTranslated(bx, by, 0);
		glScaled(0.006, 0.006, 1);
		DrawCannonBall(radius);
	}

	else {
		glTranslated(x, y + 0.03, 0);
		glScaled(0.006, 0.006, 1);
	}
	glPopMatrix();
}

double get_ground_y_by(double x)
{
	return 0.1 * sin(10 * x);
}

bool is_ground(double x, double y)
{
	return y <= get_ground_y_by(x);
}

void DrawGround()
{
	double x, y;

	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (x = -1; x <= 1; x += 0.01)
	{
		y = get_ground_y_by(x);
		glVertex2d(x, y);
	}
	glEnd();
}
void display()
{
	double y, x;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	DrawGround();
	glPushMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, -1, 0);
	glPopMatrix();
	x = 0.6 - (angle * PI / 180) * 0.03;
	y = 0.1 * sin(10 * x);
	beta = atan(cos(10 * x));
	if (!is_moving)
	{
		x2 = x;
		y2 = y;
		direction2 = direction;
	}

	DrawTank(x, y);
	draw_exploasion();
	double radius = 0.06;
	glutSwapBuffers();
}

int i = 0;

void idle()
{
	i += 1;
	if (i % 30000 == 0) {
		angle += 1.5 * direction;

		if (is_moving)
		{
			t += 0.005;
			bx = bx0 + vx * t;
			by = by0 + vy * t + a * t * t;
			double ground_bullet_height = 0.1 * sin(10 * bx);

			if (by <= ground_bullet_height) {
				is_moving = false;
				is_exploading = 1;
				exploasion_x = bx;
				exploasion_y = by;
				exploasion_timer = 0.01;
			}

		}
		glutPostRedisplay();
	}
}


void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		beta2 = beta;
		t = 0;
		is_moving = true;
		vx = cos(beta2 + PI);
		vy = sin(beta2 + PI);

		double newx = 0.6 - (angle * PI / 180) * 0.03 - 0.13 / 2;
		double newy = 0.1 * sin(10 * newx);
		double tank_x = -2 * radius - 0.01;
		double tank_y = 2 * radius + 0.01;
		double cos_b2 = cos(beta2);
		double sin_b2 = sin(beta2);
		bx0 = (newx);
		by0 = (newy + 0.03) + 0.13 / 2;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Tank");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	init();
	glutMainLoop();
}