#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#define N 100
#define POP 10000
#define GEN 1000

#define PRED_RATE 0.1
#define PRED_FREC 10
#define ART_FREC 5
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
void predation();
void artificial();

#endif
