#include <iostream>
#include <time.h>
#include <GL/glut.h>

#include "func_GC.cpp"

using namespace std;

#define NUM_POINTS 600000

int win;

punto getRandomPoint() {
	punto A;
	A.x = ((double)rand() * 10/(double)RAND_MAX) - 5;
	A.y = ((double)rand() * 10/(double)RAND_MAX) - 5;
	return A;
}

void getRandomList(int size, punto* list) {
	for (int i = 0; i < size; i++) {
		list[i] = getRandomPoint();
	}
}

void dibujaPunto(punto P) {
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
	
	punto S[NUM_POINTS];
	getRandomList(NUM_POINTS, S);
	
	glBegin(GL_POINTS);
        for (int i = 0; i < NUM_POINTS; i++) {
                dibujaPunto(S[i]);
        }
	glEnd();
	glFlush();

	glColor3f(0.0,1.0,0.0);
	punto res = minAngle(S, NUM_POINTS);
	cout << "Punto [" << res.x << ", " << res.y << "]" << endl;
	glBegin(GL_LINE);
	glVertex2f(0, 0);
	glVertex2f(res.x * 10, res.y * 10);
	glEnd();
	
	glFlush();	
}

void ejSignedArea() {
	display();

	punto A, B, C;
	glBegin(GL_POINTS);
        A = getRandomPoint(); glColor3f(1.0,0.0,0.0); dibujaPunto(A);
        B = getRandomPoint(); glColor3f(0.0,1.0,0.0); dibujaPunto(B);
        C = getRandomPoint(); glColor3f(0.0,0.0,1.0); dibujaPunto(C);
	glEnd();
	glFlush();

        cout << "Signed Area " << signedAreaABC(A, B, C) << endl;
}

void ejSortX() {
	display();

	punto S[NUM_POINTS];
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

	punto S[NUM_POINTS];
        getRandomList(NUM_POINTS, S);
	
	// Buscar un punto extremo p0
	punto p0 = lowermost (S, NUM_POINTS);

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
	
	punto S[NUM_POINTS];
	getRandomList(NUM_POINTS, S);

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
	srand((unsigned)time(0));

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	win = glutCreateWindow("Demo Geometria Computacional");
	gluOrtho2D(-7, 7, -7, 7);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyb);
	glutMainLoop();

        /*punto A, B, C, D, E;
        A.x = 1.0; A.y = 1.0;
        B.x = 4.0; B.y = 5.0;
        C.x = 4.0; C.y = 0.0;
        D.x = 5.0; D.y = 5.0;
        E.x = 0.0; E.y = 0.0;
        vector v;
        v.x = -1.0; v.y = 0.0;
        recta r;
        r.A = B; r.v = v;
        segmento s;
        s.A = E; s.B = D;

        std::cout << "La distancia entre (1, 1) y (4, 5) es " << distanceAB(A, B) << std::endl;
        std::cout << "El área entre (1, 1), (4, 5) y (4, 0) es " << signedAreaABC(A, B, C) << std::endl;
        std::cout << "Los puntos (1, 1), (0, 0) y (5, 5) están alineados " << lineABC(A, D, E) << std::endl;
        std::cout << "La distancia entre el punto (1, 1) y la recta (punto (4, 5), vector(-1, 0)) es " << distancePointLine(A, r) << std::endl;
        std::cout << "El punto (1, 1) está en el segmento que une (0, 0) con (5, 5)  " <<  pointInLineSegmentInclusion(A, s) << " pero el punto (4, 0) " << pointInLineSegmentInclusion(C, s) << std::endl;*/
}

