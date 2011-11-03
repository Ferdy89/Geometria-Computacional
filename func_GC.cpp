#include <cmath>
#include <stdlib.h>

class punto {
        public:
        float x, y;
};

class vector {
        public:
        float x, y;
};

class recta {
        public:
        punto A;        // Punto
        vector v;       // Vector
};

class segmento {
        public:
        punto A, B;
        recta getRecta()  {
                vector v;
                v.x = B.x - A.x; v.y = B.y - A.y;
                recta r;
                r.A = A; r.v = v;
                return r;
        };
};

float distanceAB(punto A, punto B){
	return sqrt(pow((B.x-A.x),2)+pow((B.y-A.y),2));
};

float signedAreaABC(punto A, punto B, punto C){
	return (((B.x-A.x)*(C.y-A.y)-((C.x-A.x)*(B.y-A.y))))/2;
};  

int lineABC(punto A, punto B, punto C){
	return !(((B.x-A.x)*(C.y-A.y)-((C.x-A.x)*(B.y-A.y))));
}; 

float distancePointLine(punto A, recta r) {
	// Primero necesitamos dos puntos de la recta
	// Asumo que me dan la forma punto y vector
	punto B, C;
	B.x = r.A.x; B.y = r.A.y;
	C.x = r.A.x + r.v.x; C.y = r.A.y + r.v.y;
	return fabs(signedAreaABC(A, B, C)) * 2 / distanceAB(B, C);
};

int pointInLineSegmentInclusion(punto A, segmento s) {
	// Primero se comprueba el bounding box
	float menorX = s.A.x < s.B.x ? s.A.x : s.B.x;
	float mayorX = s.A.x > s.B.x ? s.A.x : s.B.x;
	float menorY = s.A.y < s.B.y ? s.A.y : s.B.y;
	float mayorY = s.A.y > s.B.y ? s.A.y : s.B.y;

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

typedef struct node {
	int index;
	struct node* next;
	struct node* prev;
	int start;
} node_t;

punto* quickHull (punto* S, int size) {
	// Estructuras de datos
	node_t hull_start;
	int banned[size];
	recta arista;
	vector arista_dir;

	// Hallar puntos extremos (hull inicial)
	int min = 0, max = 0;
	for (int i = 1; i < size; i++) {
		if (S[i].x < S[min].x) {
			min = i;
		} else if (S[i].x > S[max].x) {
			max = i;
		}
	}
	banned[min] = 1;
	banned[max] = 1;	

	hull_start.index = min;
	node_t hull_2;
	hull_2.index = max;
	hull_start.next = &hull_2;
	hull_start.prev = &hull_2;
	hull_start.start = 1;
	hull_2.prev = &hull_start;
	hull_2.next = &hull_start; 

	// Para cada arista, hallar el punto exterior más alejado
	node_t* curr = &hull_start;
	float distance = 0;
	while (curr->next != NULL) {
		distance = 0;
		// Creo la arista con la que trabajar
		arista.A = S[curr->index];
		arista_dir.x = S[curr->next->index].x - S[curr->index].x;
		arista_dir.y = S[curr->next->index].y - S[curr->index].y;
		arista.v = arista_dir;
		// Itero los puntos
		for (int j = 0; j < size; j++) {
			if (!banned[j]) {
				if (signedAreaABC(S[curr->index], S[curr->next->index], S[j]) > 0) { // El punto está fuera
					if (distancePointLine(S[j], arista) > distance) { 
						max = j;
						distance = distancePointLine(S[j], arista);
					}
				} else { // El punto puede estar dentro y ser baneado
					if (pointInTriangle(S[j], S[curr->index], S[curr->next->index], S[curr->next->next->index])) {
						banned[j] = 1;
					}	
				}
			}
		}
		// Actualizo la lista de puntos baneados y la lista del hull
		banned[max] = 1;
		node_t nuevo;
		nuevo.index = max;
		nuevo.next = curr->next;
		nuevo.prev = curr;
		curr->next->prev = &nuevo;
		curr->next = &nuevo;
		curr = curr->next->next;
	}
}
