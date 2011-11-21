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

DCEL * createDCEL(int size) {
	DCEL * res = (DCEL*) calloc(1, sizeof(DCEL));
	res->max_vsize = size;
	res->max_esize = size * 6;
	res->max_fsize = size * 2;
	res->lvertex = (struct vertex*) malloc(res->max_vsize * sizeof(struct vertex));
	res->ledge = (struct edge*) malloc(res->max_esize * sizeof(struct edge));
	res->lface = (struct face*) malloc(res->max_fsize * sizeof(struct face));
	return res;
} 

vertex * addVertex(DCEL * d, point * p) {
	d->lvertex[d->vsize].x = p->x;
	d->lvertex[d->vsize].y = p->y;
	d->vsize++;
	if (d->vsize == d->max_vsize) {
		d->max_vsize = d->max_vsize ^ 2;
		d->lvertex = (struct vertex*) realloc((void*) d->lvertex, d->max_vsize * sizeof(struct vertex));
	}
	return &(d->lvertex[d->vsize - 1]); 
}


