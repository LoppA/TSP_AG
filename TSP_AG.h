#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#define N 100
#define POP 40000
#define GEN 600

#define PRED_RATE 0.1
#define PRED_FREC 10

#define CHANGE_MUT 10
#define INI_MUT 4
#define MIN_MUT 1
#define MAX_MUT 8

#define COORD_FILENAME "./data_for_tsp/coordinates.txt"
#define FIT_FILENAME "./fitnesses.txt"

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

#endif
