#include <iostream>
#define DCEL_FUNC

#ifndef STRUCTS_GC
	#include "libGC.h"
#endif

#ifndef GC_TOOLS
	#include "libGC.cpp"
#endif

using namespace std;

/*
 * Implementación en C / C++ de la estructura DCEL para Geometría Computacional
 *
 * Versión sólo para grafos conexos
 * DCEL mínimo de dos vértices y una arista (createDCEL)
 * Se puede añadir un vértice y una arista (addVertex)
 * Se puede añadir una arista con dos vértices existentes (addEdge)
 *
 */

// Funciones de manejo

DCEL * createDCEL(int size, point * p1, point * p2) {
	// Create empty DCEL and its inner structures with preconditions
	DCEL * res = (DCEL*) calloc(1, sizeof(DCEL));

	if (size < 2) size = 2;

	res->max_vsize = size;
	res->max_esize = size * 6;
	res->max_fsize = size * 2;

	res->lvertex = (struct vertex*) malloc(res->max_vsize * sizeof(struct vertex));
	res->ledge = (struct edge*) malloc(res->max_esize * sizeof(struct edge));
	res->lface = (struct face*) malloc(res->max_fsize * sizeof(struct face));

	// Initializate basic structure
	res->lvertex[0].x = p1->x;
	res->lvertex[0].y = p1->y;
	res->lvertex[0].vedge = &(res->ledge[0]);

	res->lvertex[1].x = p2->x;
	res->lvertex[1].y = p2->y;
	res->lvertex[1].vedge = &(res->ledge[1]);

	res->ledge[0].origin = &(res->lvertex[0]);
	res->ledge[0].twin = &(res->ledge[1]);
	res->ledge[0].next = &(res->ledge[1]);
	res->ledge[0].prev = &(res->ledge[1]);
	res->ledge[0].eface = &(res->lface[0]);

	res->ledge[1].origin = &(res->lvertex[1]);
	res->ledge[1].twin = &(res->ledge[0]);
	res->ledge[1].next = &(res->ledge[0]);
	res->ledge[1].prev = &(res->ledge[0]);
	res->ledge[1].eface = &(res->lface[0]);

	res->lface[0].outer = &(res->ledge[1]);

	res->vsize = 2;
	res->esize = 2;
	res->fsize = 1;

	return res;
} 

struct vertex* addVertex(DCEL * d, point * p, struct edge * e) {
	// Add vertex
	d->lvertex[d->vsize].x = p->x;
	d->lvertex[d->vsize].y = p->y;
	d->lvertex[d->vsize].vedge = &(d->ledge[d->esize + 1]);
	d->vsize++;

	// Check DCEL's size
	/*if (d->vsize >= d->max_vsize) {
		d->max_vsize = d->max_vsize ^ 2;
		d->lvertex = (struct vertex*) realloc((void*) d->lvertex, d->max_vsize * sizeof(struct vertex));
	}

	if (d->esize + 2 >= d->max_esize) {
		d->max_esize = d->max_esize ^ 2;
		d->ledge = (struct edge*) realloc((void*) d->ledge, d->max_esize * sizeof(struct edge));
	}*/

	// Add edges
	d->ledge[d->esize].origin = e->origin;
	d->ledge[d->esize].twin = &(d->ledge[d->esize + 1]);
	d->ledge[d->esize].next = &(d->ledge[d->esize + 1]);
	d->ledge[d->esize].prev = e->prev;
	d->ledge[d->esize].eface = e->eface;
	d->esize++;

	d->ledge[d->esize].origin = &(d->lvertex[d->vsize - 1]);
	d->ledge[d->esize].twin = &(d->ledge[d->esize - 1]);
	d->ledge[d->esize].next = e;
	d->ledge[d->esize].prev = &(d->ledge[d->esize - 1]);
	d->ledge[d->esize].eface = e->eface;
	d->esize++;

	e->prev->next = &(d->ledge[d->esize - 2]);
	e->prev = &(d->ledge[d->esize - 1]);
	
	return &(d->lvertex[d->vsize - 1]); 
}
struct edge * addEdge(DCEL * d, struct edge * e1, struct edge * e2) {
	// Check DCEL's size
	/*if (d->esize + 2 >= d->max_esize) {
		d->max_esize = d->max_esize ^ 2;
		d->ledge = (struct edge*) realloc((void*) d->ledge, d->max_esize * sizeof(struct edge));
	}
	if (d->fsize + 1 >= d->max_fsize) {
		d->max_fsize = d->max_fsize ^ 2;
		d->lface = (struct face*) realloc((void*) d->lface, d->max_fsize * sizeof(struct face));
	}*/

	// Add face
	d->lface[d->fsize].outer = e2;
	d->fsize++;

	// Add edges
	d->ledge[d->esize].origin = e1->origin;
	d->ledge[d->esize].twin = &(d->ledge[d->esize + 1]);
	d->ledge[d->esize].next = e2;
	d->ledge[d->esize].prev = e1->prev;
	d->ledge[d->esize].eface = &(d->lface[d->fsize - 1]);
	d->esize++;

	d->ledge[d->esize].origin = e2->origin;
	d->ledge[d->esize].twin = &(d->ledge[d->esize - 1]);
	d->ledge[d->esize].next = e1;
	d->ledge[d->esize].prev = e2->prev;
	d->ledge[d->esize].eface = e1->eface;
	d->esize++;

	e1->prev->next = &(d->ledge[d->esize - 2]);
	e1->prev = &(d->ledge[d->esize - 1]);

	e2->prev->next = &(d->ledge[d->esize - 1]);
	e2->prev = &(d->ledge[d->esize - 2]);

	struct edge * actual = e2;
	do {
		actual->eface = &(d->lface[d->fsize - 1]);
		actual->eface->outer = actual;
		actual = actual->next;
	} while (actual != e2);

	return &(d->ledge[d->esize - 1]);
}

void flip(struct edge * e) {
	// Dejo el cuadrilatero
	e->next->prev = e->twin->prev;
	e->twin->prev->next = e->next;

	e->prev->next = e->twin->next;
	e->twin->next->prev = e->prev;

	// Anclo los nuevos origenes
	e->origin = e->twin->prev->origin;
	e->twin->origin = e->prev->origin;

	// Anclo las aristas al cuadrilatero
	e->prev = e->twin->next;
	e->next = e->next->next;

	e->twin->next = e->twin->prev;
	e->twin->prev = e->next->prev;

	// Anclo el cuadrilatero a las aristas
	e->next->prev = e;
	e->prev->next = e;

	e->twin->next->prev = e->twin;
	e->twin->prev->next = e->twin;

	// Apaño los cambios de cara
	e->eface = e->prev->eface;
	e->next->eface = e->eface;

	e->eface->outer = e;

	e->twin->eface = e->twin->prev->eface;
	e->twin->next->eface = e->twin->eface;

	e->twin->eface->outer = e->twin;
}

void printDCEL(DCEL * d) { // DEBUG ONLY
	int i;
	cout << "Vertex" << endl;
	for (i = 0; i < d->vsize; i++) {
		cout << "(" << &(d->lvertex[i]) << ") " << d->lvertex[i].x << ", " << d->lvertex[i].y << ", " << d->lvertex[i].vedge << endl;
	}
	cout << "Edges" << endl;
	for (i = 0; i < d->esize; i++) {
		cout << "(" << &(d->ledge[i]) << ") " << d->ledge[i].origin << ", " << d->ledge[i].twin << ", " << d->ledge[i].next << ", " << d->ledge[i].prev << ", " << d->ledge[i].eface << endl;
	}
	cout << "Faces" << endl;
	for (i = 0; i < d->fsize; i++) {
		cout << "(" << &(d->lface[i]) << ") " << d->lface[i].outer << endl;
	}
}
