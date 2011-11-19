#include <cmath>
#include <stdlib.h>

/*
* Estucturas de datos
*/

typedef struct {
	double x, y;
} punto;

typedef struct {
	double x, y;
} vector;

typedef struct {
	punto A;
	vector v;
} recta;

typedef struct {
	punto A, B;
} segmento;

struct point_node {
	struct point_node * ant;
	struct point_node * sig;
	punto p;
};

typedef struct point_node pnode;

/*
* Funciones geométricas
*/

double distanceAB(punto A, punto B){
	return sqrt(pow((B.x-A.x),2)+pow((B.y-A.y),2));
};

double signedAreaABC(punto A, punto B, punto C){
	return (((B.x-A.x)*(C.y-A.y)-((C.x-A.x)*(B.y-A.y))))/2;
};  

int lineABC(punto A, punto B, punto C){
	return !(((B.x-A.x)*(C.y-A.y)-((C.x-A.x)*(B.y-A.y))));
}; 

double distancePointLine(punto A, recta r) {
	// Primero necesitamos dos puntos de la recta
	// Asumo que me dan la forma punto y vector
	punto B, C;
	B.x = r.A.x; B.y = r.A.y;
	C.x = r.A.x + r.v.x; C.y = r.A.y + r.v.y;
	return fabs(signedAreaABC(A, B, C)) * 2 / distanceAB(B, C);
};

int pointInLineSegmentInclusion(punto A, segmento s) {
	// Primero se comprueba el bounding box
	double menorX = s.A.x < s.B.x ? s.A.x : s.B.x;
	double mayorX = s.A.x > s.B.x ? s.A.x : s.B.x;
	double menorY = s.A.y < s.B.y ? s.A.y : s.B.y;
	double mayorY = s.A.y > s.B.y ? s.A.y : s.B.y;

	if (menorX > A.x || mayorX < A.x || menorY > A.y || mayorY < A.y) {
		return 0;
	} else {	// Está en el bounding box del segmento
		return signedAreaABC(A, s.A, s.B) == 0;
	}
};

int pointInTriangle(punto A, punto T1, punto T2, punto T3) {
        if ((signedAreaABC(T1, T2, A) <= 0 && signedAreaABC(T3, T3, A) <= 0 && signedAreaABC(T3, T1, A) <= 0) ||
                (signedAreaABC(T1, T2, A) >= 0 && signedAreaABC(T2, T3, A) >= 0 && signedAreaABC(T3, T1, A) >= 0))
                return 1;
        else
                return 0;
} 

int segmentsIntersection (segmento R, segmento S) {
        if ((signedAreaABC(R.A, R.B, S.A) * signedAreaABC(R.A, R.B, S.A) <= 0) &&
                (signedAreaABC(S.A, S.B, R.A) * signedAreaABC(S.A, S.B, R.B) <= 0))
                return 1;
        else
                return 0;
}

punto rightmost(punto* S, int size) {
	punto most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.x > S[i].x ? most : S[i];
	}
	return most;
}

punto leftmost(punto* S, int size) {
	punto most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.x < S[i].x ? most : S[i];
	}
	return most;
}

punto uppermost(punto* S, int size) {
	punto most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.y > S[i].y ? most : S[i];
	}
	return most;
}

punto lowermost(punto* S, int size) {
	punto most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.y < S[i].y ? most : S[i];
	}
	return most;
}

punto vectormost(punto* S, int size, vector V) {
	punto most = S[0];
	for (int i = 1; i < size; i++) {
		most = most.x * V.x + most.y * V.y > S[i].x * V.x + S[i].y * V.y ? most : S[i];
	}
	return most;
}

punto furthestPoint (punto* S, int size) {
	punto further = S[0];
	for (int i = 1; i < size; i++) {
		further = pow(further.x, 2) + pow(further.y, 2) > pow(S[i].x, 2) + pow(S[i].y, 2) ? further : S[i]; 
	}
	return further;
}

punto minAngle (punto* S, int size) {
	punto M = S[0];
	punto O; O.x = 0; O.y = 0;
	punto U; U.x = 1; U.y = 0;
	int soloArriba = signedAreaABC(M, O, U);
	
	for (int i = 1; i < size; i++) {
		if (soloArriba) {
			if (signedAreaABC(S[i], O, U) > 0 && (signedAreaABC(M, O, S[i]) > 0 || (signedAreaABC(M, O, S[i]) == 0 && distanceAB(O, S[i]) > distanceAB(O, M)))) {
				M = S[i];	
			}
		} else { // Aún no hemos encontrado un punto en el primer o segundo cuadrante
			if (signedAreaABC(S[i], O, U) > 0) { // Este punto es del primer o segundo cuadrante
				M = S[i];
				soloArriba = 1;
			} else if (signedAreaABC(M, O, S[i]) > 0 || (signedAreaABC(M, O, S[i]) == 0 && distanceAB(O, S[i]) > distanceAB(O, M))) {
				M = S[i];
			}
		}
	}
	return M;
}

int compX (const void * p1, const void * p2) {
	if (((*(punto *)p1).x - (*(punto *)p2).x) > 0)
		return 1;
	else if (((*(punto *)p1).x - (*(punto *)p2).x) == 0) {
		return ((*(punto *)p1).y - (*(punto *)p2).y) > 0;
	} else
		return -1;
}

void sortByX(punto* S, int size) {
	qsort(S, size, sizeof(punto), compX);
}

punto P0;

int compAngle (const void * p1, const void * p2) {
	/* MUY IMPORTANTE */
	/* Esta funcion espera que P0 sea el punto mas bajo de todos */
	/* De no ser asi los resultados son impredecibles */
	if (signedAreaABC(P0, *(punto *) p1, *(punto *) p2) > 0) {
		return -1 ;
	} else if (signedAreaABC(P0, *(punto *) p1, *(punto *) p2) < 0){
		return 1;
	} else {
		if (distanceAB(P0, *(punto *) p1) > distanceAB(P0, *(punto *) p2))
			return 1;
		else
			return -1;
	}
}

void sortByAngle (punto* S, int size, punto origin) {
	P0 = origin;
	qsort(S, size, sizeof(punto), compAngle);
}

pnode* graham (punto* S, int size) {	
	// Buscar un punto extremo p0
	punto p0 = lowermost (S, size);

	// Ordenar la lista angularmente positivamente alrededor de p0
	sortByAngle (S, size, p0);

	pnode * head, * tail = (pnode*) calloc(1, sizeof(pnode));
	tail->p = p0;
	head = tail;

	int lturn;
	pnode * newp, * del;
	// Recorrer la lista ordenada
	for (int i = 0; i < size; i++) {
		// Añadir el siguiente punto al convex hull
		newp = (pnode*) calloc(1, sizeof(pnode));
		newp->p = S[i];
		newp->ant = tail;
		tail->sig = newp;
		tail = newp;	
		
		lturn = 0;
		while (!lturn) {
			// Si el giro es a la izquierda, continuar
			if (tail->ant->ant == NULL) {
				lturn = 1;
			} else if (signedAreaABC(tail->ant->ant->p, tail->ant->p, tail->p) > 0) {
				lturn = 1;
			}
			
			// Si el giro es a la derecha, eliminar el punto anterior del convex hull y volver al paso anterior
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
