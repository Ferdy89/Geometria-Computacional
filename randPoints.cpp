#include <time.h>
//#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define NUM_POINTS 1000000

point getRandomPoint() {
	point A;
	A.x = ((double)rand() * 10/(double)RAND_MAX) - 5;
	A.y = ((double)rand() * 10/(double)RAND_MAX) - 5;
	return A;
}

point* getRandomList(int size) {
	gsl_rng_env_setup();

	gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);
	gsl_rng_set(r, (unsigned) time(NULL));	

	point * list = (point*) malloc(size * sizeof(point));
	if (list == NULL) {
		exit(-1);
	}
	for (int i = 0; i < size; i++) {
		list[i].x = gsl_rng_uniform(r) * 10 - 5;
                list[i].y = gsl_rng_uniform(r) * 10 - 5;
	}
	return list;
}

point * getNormalList(int size) {
	gsl_rng_env_setup();

	gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);
	gsl_rng_set(r, (unsigned) time(NULL));

	point * list = (point*) malloc(size * sizeof(point));
	if (list == NULL) {
		exit(-1);
	}

	for (int i = 0; i < size; i++) {
		list[i].x = gsl_ran_gaussian(r, 1);
		list[i].y = gsl_ran_gaussian(r, 1);
	}

	gsl_rng_free (r);
	
	return list;
}
