#include "TSP_AG.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

int main () {
	srand(time(NULL));

	for (int i = 0; i < POP; i++) {
		for (int j = 0; j < POP; j++) {
			double a = rand(), b = rand();
			printf ("%.5lf ", a/b);
		}
		printf ("\n");
	}

	return 0;
}
