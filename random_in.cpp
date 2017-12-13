#include "TSP_AG.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

int main () {
	srand(time(NULL));

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double a = rand(), b = rand();
			printf ("%.5lf ", a/b);
		}
		printf ("\n");
	}

	return 0;
}
