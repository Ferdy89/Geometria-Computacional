#define STRUCTS_GC

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

struct vertex{
	double x, y;
	struct edge * vedge;
};

struct face {
	struct edge * inner, * outer;
};

struct edge {
	struct vertex * origin;
	struct edge * twin, * next, * prev;
	struct face * eface;
};

typedef struct {
	int vsize, esize, fsize;
	int max_vsize, max_esize, max_fsize;
	struct vertex * lvertex;
	struct edge * ledge;
	struct face * lface;
} DCEL;


