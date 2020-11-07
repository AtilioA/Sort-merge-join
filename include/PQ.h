#ifndef __PQ_H
#define __PQ_H
#include <stdbool.h>
#include <stdlib.h>
#define exch(A, B)   \
    {                \
        void *t = A; \
        A = B;       \
        B = t;       \
    }

typedef struct pq PQ;

PQ *PQ_init(int n);

void PQ_insert(PQ *pq, void *item, int (*compare)(const void *, const void *));

void *PQ_delmin(PQ *pq, int (*compare)(const void *, const void *));

void *PQ_min(PQ *pq);

void PQ_FixPri(PQ *pq, int (*compare)(const void *, const void *));

bool PQ_empty(PQ *pq);

int PQ_size(PQ *pq);

void PQ_finish(PQ *pq);

#endif