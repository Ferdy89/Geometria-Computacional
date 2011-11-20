#include <time.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define NUM_POINTS 100000

point getRandomPoint() {
	point A;
	A.x = ((double)rand() * 10/(double)RAND_MAX) - 5;
	A.y = ((double)rand() * 10/(double)RAND_MAX) - 5;
	return A;
}

void getRandomList(int size, point* list) {
	srand((unsigned)time(0));
	for (int i = 0; i < size; i++) {
		list[i] = getRandomPoint();
	}
}

gsl_rng* r;

point getNormalPoint() {
	point A;
	A.x = gsl_ran_gaussian(r, 1);
	A.y = gsl_ran_gaussian(r, 1);
	return A;
}

void getNormalList(int size, point* list) {
	r = gsl_rng_alloc(gsl_rng_taus);
	gsl_rng_set(r, (unsigned)time(0));
	for (int i = 0; i < size; i++) {
		list[i] = getNormalPoint();
	}
}
