#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#define N 10
#define POP 100
#define GEN 20

#define MUT 0.5

struct Gen {
	int pos[N];
	double fitness;

	Gen ();

	void print ();

	double rate ();

	void operator = (const Gen &other);

	bool operator < (const Gen &other) const;
};

bool raffle (double);
Gen cross (Gen, Gen);
void reproduction();

#endif
