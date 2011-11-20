#include <iostream>
#include <GL/glut.h>
#include "func_GC.cpp"
#include "randPoints.cpp"

using namespace std;

int win;

void dibujaPunto(point P) {
	glVertex2f(P.x, P.y);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINE);
	glVertex2f(-6.0,0.0);
	glVertex2f(6.0,0.0);
	glEnd();
	glBegin(GL_LINE);
	glVertex2f(0.0,-6.0);
	glVertex2f(0.0,6.0);
	glEnd();
	//glPointSize(5);
	glFlush();
} 

void ejMinAngle() {
	display();
	
	point S[NUM_POINTS];
	getRandomList(NUM_POINTS, S);
	
	glBegin(GL_POINTS);
        for (int i = 0; i < NUM_POINTS; i++) {
                dibujaPunto(S[i]);
        }
	glEnd();
	glFlush();

	glColor3f(0.0,1.0,0.0);
	point res = minAngle(S, NUM_POINTS);
	cout << "Punto [" << res.x << ", " << res.y << "]" << endl;
	glBegin(GL_LINE);
	glVertex2f(0, 0);
	glVertex2f(res.x * 10, res.y * 10);
	glEnd();
	
	glFlush();	
}

void ejSignedArea() {
	display();

	point A, B, C;
	glBegin(GL_POINTS);
        A = getRandomPoint(); glColor3f(1.0,0.0,0.0); dibujaPunto(A);
        B = getRandomPoint(); glColor3f(0.0,1.0,0.0); dibujaPunto(B);
        C = getRandomPoint(); glColor3f(0.0,0.0,1.0); dibujaPunto(C);
	glEnd();
	glFlush();

        cout << "Signed Area " << area(A, B, C) << endl;
}

void ejSortX() {
	display();

	point S[NUM_POINTS];
        getRandomList(NUM_POINTS, S);
	
	sortByX(S, NUM_POINTS);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < NUM_POINTS; i++) {
		glVertex2f(S[i].x, S[i].y);
	}
	glEnd();
	glFlush();
}

void ejSortAngle() {
	display();

	point S[NUM_POINTS];
        getRandomList(NUM_POINTS, S);
	
	// Buscar un point extremo p0
	point p0 = lowermost (S, NUM_POINTS);

	// Ordenar la lista angularmente positivamente alrededor de p0
	sortByAngle (S, NUM_POINTS, p0);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < NUM_POINTS; i++) {
		glVertex2f(S[i].x, S[i].y);
	}
	glEnd();
	glFlush();
}

void ejGraham() {
	display();
	
	point S[NUM_POINTS];
	getNormalList(NUM_POINTS, S);

	glBegin(GL_POINTS);
	for (int i = 0; i < NUM_POINTS; i++) {
		dibujaPunto(S[i]);
	}
	glEnd();
	glFlush();

	pnode* hull_head = graham(S, NUM_POINTS);

	glBegin(GL_LINE_LOOP);
	while(hull_head != NULL) {
		glVertex2f(hull_head->p.x, hull_head->p.y);
		hull_head = hull_head->sig;
	}
	glEnd();
	glFlush();
}

void keyb(unsigned char key, int x, int y){
	if (key == 13) {
		display();
	} else if (key == 'a') {
		ejSignedArea();
	} else if (key == 'n') {
		ejMinAngle();
	} else if (key == 's') {
		ejSortX();
	} else if (key == 'q') {
		glutDestroyWindow(win);
	} else if (key == 'g') {
		ejGraham();
	} else if (key == 'w') {
		ejSortAngle();
	}
}

int main(int argc, char **argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	win = glutCreateWindow("Demo Geometria Computacional");
	gluOrtho2D(-7, 7, -7, 7);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyb);
	glutMainLoop();
}

