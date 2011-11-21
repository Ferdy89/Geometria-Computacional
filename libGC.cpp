#include <cmath>

using namespace std;

/*
* Estucturas de datos
*/

typedef struct {
	double x, y;
} point;

typedef struct {
	double x, y;
} vector;

typedef struct {
	point A;
	vector v;
} line;

typedef struct {
	point A, B;
} segmento;

struct point_node {
	struct point_node * ant;
	struct point_node * sig;
	point p;
};

typedef struct point_node pnode;

/*
* Funciones geométricas
*/

double dist(point A, point B){
	return sqrt(pow((B.x-A.x),2)+pow((B.y-A.y),2));
};

double area(point A, point B, point C){
	return (((B.x-A.x)*(C.y-A.y)-((C.x-A.x)*(B.y-A.y))))/2;
};  

int lined(point A, point B, point C){
	return !(((B.x-A.x)*(C.y-A.y)-((C.x-A.x)*(B.y-A.y))));
}; 

double dPointLine(point A, line r) {
	// Primero necesitamos dos puntos de la line
	// Asumo que me dan la forma point y vector
	point B, C;
	B.x = r.A.x; B.y = r.A.y;
	C.x = r.A.x + r.v.x; C.y = r.A.y + r.v.y;
	return fabs(area(A, B, C)) * 2 / dist(B, C);
};

int inSegment(point A, segmento s) {
	// Primero se comprueba el bounding box
	double menorX = s.A.x < s.B.x ? s.A.x : s.B.x;
	double mayorX = s.A.x > s.B.x ? s.A.x : s.B.x;
	double menorY = s.A.y < s.B.y ? s.A.y : s.B.y;
	double mayorY = s.A.y > s.B.y ? s.A.y : s.B.y;

	if (menorX > A.x || mayorX < A.x || menorY > A.y || mayorY < A.y) {
		return 0;
	} else {	// Está en el bounding box del segmento
		return area(A, s.A, s.B) == 0;
	}
};

int inTriangle(point A, point T1, point T2, point T3) {
        if ((area(T1, T2, A) <= 0 && area(T3, T3, A) <= 0 && area(T3, T1, A) <= 0) ||
                (area(T1, T2, A) >= 0 && area(T2, T3, A) >= 0 && area(T3, T1, A) >= 0))
                return 1;
        else
                return 0;
} 

int intersection (segmento R, segmento S) {
        if ((area(R.A, R.B, S.A) * area(R.A, R.B, S.A) <= 0) &&
                (area(S.A, S.B, R.A) * area(S.A, S.B, R.B) <= 0))
                return 1;
        else
                return 0;
}

point rightmost(point* S, int size) {
	point most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.x > S[i].x ? most : S[i];
	}
	return most;
}

point leftmost(point* S, int size) {
	point most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.x < S[i].x ? most : S[i];
	}
	return most;
}

point uppermost(point* S, int size) {
	point most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.y > S[i].y ? most : S[i];
	}
	return most;
}

point lowermost(point* S, int size) {
	point most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.y < S[i].y ? most : S[i];
	}
	return most;
}

point vectormost(point* S, int size, vector V) {
	point most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.x * V.x + most.y * V.y > S[i].x * V.x + S[i].y * V.y ? most : S[i];
	}
	return most;
}

point furthest (point* S, int size) {
	point further = S[0];
	for (int i = 1; i < size; i++) {
		further = pow(further.x, 2) + pow(further.y, 2) > pow(S[i].x, 2) + pow(S[i].y, 2) ? further : S[i]; 
	}
	return further;
}

point minAngle (point* S, int size) {
	point M = S[0];
	point O; O.x = 0; O.y = 0;
	point U; U.x = 1; U.y = 0;
	int soloArriba = area(M, O, U);
	
	for (int i = 1; i < size; i++) {
		if (soloArriba) {
			if (area(S[i], O, U) > 0 && (area(M, O, S[i]) > 0 || (area(M, O, S[i]) == 0 && dist(O, S[i]) > dist(O, M)))) {
				M = S[i];	
			}
		} else { // Aún no hemos encontrado un point en el primer o segundo cuadrante
			if (area(S[i], O, U) > 0) { // Este point es del primer o segundo cuadrante
				M = S[i];
				soloArriba = 1;
			} else if (area(M, O, S[i]) > 0 || (area(M, O, S[i]) == 0 && dist(O, S[i]) > dist(O, M))) {
				M = S[i];
			}
		}
	}
	return M;
}

int compX (const void * p1, const void * p2) {
	if (((*(point *)p1).x - (*(point *)p2).x) > 0)
		return 1;
	else if (((*(point *)p1).x - (*(point *)p2).x) == 0) {
		return ((*(point *)p1).y - (*(point *)p2).y) > 0;
	} else
		return -1;
}

void sortByX(point* S, int size) {
	qsort(S, size, sizeof(point), compX);
}

point P0;

int compAngle (const void * p1, const void * p2) {
	/* MUY IMPORTANTE */
	/* Esta funcion espera que P0 sea el point mas bajo de todos */
	/* De no ser asi los resultados son impredecibles */
	if (area(P0, *(point *) p1, *(point *) p2) > 0) {
		return -1 ;
	} else if (area(P0, *(point *) p1, *(point *) p2) < 0){
		return 1;
	} else {
		if (dist(P0, *(point *) p1) > dist(P0, *(point *) p2))
			return 1;
		else
			return -1;
	}
}

void sortByAngle (point* S, int size, point origin) {
	P0 = origin;
	qsort(S, size, sizeof(point), compAngle);
}

pnode* graham (point* S, int size) {	
	// Buscar un point extremo p0
	point p0 = lowermost (S, size);

	// Ordenar la lista angularmente positivamente alrededor de p0
	sortByAngle (S, size, p0);

	pnode * head, * tail = (pnode*) calloc(1, sizeof(pnode));
	if (tail == NULL) {
		exit(-1);
	}
	tail->p = p0;
	head = tail;

	int lturn;
	pnode * newp, * del;
	// Recorrer la lista ordenada
	for (int i = 0; i < size; i++) {
		// Añadir el siguiente point al convex hull
		newp = (pnode*) calloc(1, sizeof(pnode));
		if (newp == NULL) {
			exit(-1);
		}
		newp->p = S[i];
		newp->ant = tail;
		tail->sig = newp;
		tail = newp;	
		
		lturn = 0;
		while (!lturn) {
			// Si el giro es a la izquierda, continuar
			if (tail->ant->ant == NULL) {
				lturn = 1;
			} else if (area(tail->ant->ant->p, tail->ant->p, tail->p) > 0) {
				lturn = 1;
			}
			
			// Si el giro es a la derecha, eliminar el point anterior del convex hull y volver al paso anterior
			else {
				del = tail->ant;
				del->ant->sig = del->sig;
				del->sig->ant = del->ant;
				free((void*) del); 
			}
		}
	}

	return head;
}
