#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#define N 10
#define POP 10
#define GEN 10

struct Gen {
	int pos[N];
	double fitness;

	Gen ();

	void print ();

	double rate ();

	void operator = (const Gen &other);
};

#endif
