/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs

Referencias:
Funcoes de C/C++:
	http://www.cplusplus.com/
Copia online do Red Book (OpenGL Programming Guide):
	http://fly.cc.fer.hr/~unreal/theredbook/
Referencia online para os comandos OpenGL (Man pages):
	http://www.opengl.org/sdk/docs/man/

-----------------------------------------------------------------------------
*/

#include "openGL_tutorial.h"
#include <vector>
#include <math.h>

using namespace std;

double INTERVALO = 0.01;

class Point{
public:
	long double x,y;

	Point(long double a, long double b){
		x = a;
		y = b;
	}
};
vector<Point> points;

GLfloat mouse_x, mouse_y;
GLfloat window_width = 800.0;
GLfloat window_height = 800.0;

void myreshape (GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	window_width = (GLfloat) w;
	window_height = (GLfloat) h;
	glOrtho(0, window_width, window_height, 0, -1.0, -1.0);
}

// aqui o sistema de coordenadas da tela está variando de -1 a 1 no eixo x e y

vector<Point> bezierPoints;
void drawCurve(vector<Point> points, double t){
	if(points.size() == 1){
		bezierPoints.push_back(*new Point(points[0].x,points[0].y));
		glVertex3f(points[0].x, points[0].y, 0);
	}else{
		vector<Point> newPoints;
		for(int i =0; i < points.size() -1; i++){
			double x = points[i].x*(1-t) + points[i+1].x*t;
			double y = points[i].y*(1-t) + points[i+1].y*t;

			
			Point p = Point(x,y);
			newPoints.push_back(p);
		}
		drawCurve(newPoints, t);
	}
}

GLfloat pt[3];
void mydisplay()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	long double x,y;

	glColor3f(0, 1, 1);
	glPointSize(3.0);

	for(int q = 0; q < points.size(); q++){
	
		x = points.at(q).x;
		y = points.at(q).y;

		glBegin(GL_POINTS);
		pt[0] = x;
		pt[1] = y;
		pt[2] = 0;
		glVertex3fv(pt);
		glEnd();

	}

	glColor3f(1, 1, 1);

	if(points.size() > 1){
		bezierPoints.clear();
		glBegin(GL_POINTS);
		for(double t=0; t < 1; t += INTERVALO){
			drawCurve(points, t);
		}
		glEnd();

		glBegin(GL_LINES);
		for(int k =0;k<bezierPoints.size()-1;k++){
			glColor3f(1, (double)k/bezierPoints.size(), 1);
			glVertex2f(bezierPoints.at(k).x, bezierPoints.at(k).y);
			glVertex2f(bezierPoints.at(k+1).x, bezierPoints.at(k+1).y);
		}
		glVertex2f(bezierPoints.at(bezierPoints.size()-1).x, bezierPoints.at(bezierPoints.size()-1).y);
		glVertex2f(points.at(points.size()-1).x, points.at(points.size()-1).y);
		glEnd();
	}

	glFlush();
	glutPostRedisplay();
}

bool mov;
int pointNumber;
void handleMotion(int x, int y)
{
	float X,Y;

	if(mov){

		X = ((x*2)/window_width) -1;
		Y = -((y*2)/window_height) +1;
		points.at(pointNumber).x = X;
		points.at(pointNumber).y = Y;
	}
	
}

void handleMouse(int btn, int state, int x, int y)
{		
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		long double X,Y;

		X = ((x*2)/window_width) -1;
		Y = -((y*2)/window_height) +1;
		Point pt = *new Point(X,Y);		

		mov = false;
		for(int k=0;k<points.size();k++){
			if(abs(points.at(k).x - X)<0.01 && abs(points.at(k).y -Y )<0.01){
				mov = true;
				pointNumber = k;
				break;
			}
		}
		if(!mov)points.push_back(pt);
	}

	if(btn == GLUT_LEFT_BUTTON &&  state == GLUT_UP){
		mov = false;
	}	
}

void hadleKeyboard(unsigned char key, int x, int y)
{	
	if(key == 's'){
		if(INTERVALO + 0.1 < 1)	INTERVALO += 0.1;
	}
	if(key == 'a'){
		if(INTERVALO - 0.1 > 0)	INTERVALO -= 0.1;
	}
	if(key == ESC){
		exit(0);
	}
}

void hadleSpecialKeyboard(int key, int x, int y)
{
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("OpenGL");

	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	glutMainLoop();
	return 0;
}
