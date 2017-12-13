#ifndef __TSP_AG_H__
#define __TSP_AG_H__

#define N 25
#define POP 10000
#define GEN 10000

#define PRED_RATE 0.3
#define PRED_FREC 10

#define CHANGE_MUT 3
#define INI_MUT 0.5
#define MAX_MUT 0.9
#define INC_MUT 0.1
#define N_INI_MUT 5
#define N_MIN_MUT 1
#define N_MAX_MUT 9

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
