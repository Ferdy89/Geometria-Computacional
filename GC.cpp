#include <iostream>
#include <GL/glut.h>
#include "libGC.cpp"
#include "randPoints.cpp"

#ifndef DCEL_FUNC
	#include "DCEL.cpp"
#endif

#include "defines.cpp"
#include <time.h>

using namespace std;

int win;
int showp = 1;
clock_t start, end;

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
	glPointSize(POINT_WIDTH);
	glFlush();
} 

void ejMinAngle() {
	display();
	
	point* S = getRandomList(NUM_POINTS);
	
	if (showp) {
		glBegin(GL_POINTS);
		for (int i = 0; i < NUM_POINTS; i++) {
			dibujaPunto(S[i]);
		}
		glEnd();
		glFlush();
	}

	glColor3f(0.0,1.0,0.0);
	point res = minAngle(S, NUM_POINTS);
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

        point* S = getRandomList(NUM_POINTS);
	
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

        point* S = getRandomList(NUM_POINTS);
	
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
	
	point* S = getNormalList(NUM_POINTS);

	if (showp) {
		glBegin(GL_POINTS);
		for (int i = 0; i < NUM_POINTS; i++) {
			dibujaPunto(S[i]);
		}
		glEnd();
		glFlush();
	}

	pnode* hull_head = graham(S, NUM_POINTS);

	glBegin(GL_LINE_LOOP);
	while(hull_head != NULL) {
		glVertex2f(hull_head->p.x, hull_head->p.y);
		hull_head = hull_head->sig;
	}
	glEnd();
	glFlush();
}

DCEL * D;

void drawDCEL(DCEL * d) {
	display();

	for (int i = 0; i < d->esize; i++) {
		glBegin(GL_LINE);
		glVertex2f(d->ledge[i].origin->x, d->ledge[i].origin->y);
		glVertex2f(d->ledge[i].next->origin->x, d->ledge[i].next->origin->y);
		glEnd();
	}
	glFlush();
}

void ejDCEL() {
	start = clock();
	point* S = getRandomList(NUM_POINTS);
	end = clock();
	cout << "Getting " << NUM_POINTS << " random points... " << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

	start = clock();
	D = triangulate(S, NUM_POINTS);
	end = clock();
	cout << "Triangulating " << NUM_POINTS << " points... " << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

	drawDCEL(D);
	// printDCEL(D);
}

void ejCentre() {
	display();
	point * S = getRandomList(3);
	glBegin(GL_LINE_LOOP);
	glVertex2f(S[0].x, S[0].y);
	glVertex2f(S[1].x, S[1].y);
	glVertex2f(S[2].x, S[2].y);
	glEnd();
	DCEL * D = triangulate(S, 3);
	point res = circumcentre(&(D->lvertex[0]), &(D->lvertex[1]), &(D->lvertex[2]));
	glBegin(GL_POINT);
	glVertex2f(res.x, res.y);
	glEnd();
	glFlush();
}

void ejDelaunay() {
	start = clock();
	Delaunay(D);
	end = clock();
	cout << "Getting Delaunay from " << NUM_POINTS << " points... " << (double)(end-start)/CLOCKS_PER_SEC << " seconds" << endl;

	drawDCEL(D);
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
	} else if (key == 'p') {
		//showp = !showp;
		if (D != NULL)
			printDCEL(D);
	} else if (key == 'd') {
		ejDCEL();
	} else if (key == 'c') {
		ejCentre();
	} else if (key == 'y') {
		ejDelaunay();
	}}

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

