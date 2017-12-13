#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#define N 10
#define POP 10

struct Gen {
	int pos[N];

	// O(N**2)
	Gen ();

	void print ();
};

#endif
