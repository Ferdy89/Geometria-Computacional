struct vertex{
	double x, y;
	struct edge * edge;
};

struct face {
	struct edge * inner, * outer;
};

struct edge {
	struct vertex * origin;
	struct edge * twin, * next, * prev;
	struct face * face;
};

typedef struct {
	struct vertex * lvertex;
	struct edge * ledge;
	struct face * lface;
} DCEL;
