#include "../include/PQ.h"
#include <stdio.h>

struct pq
{
    void **vec;
    int size;
};

PQ *PQ_init(int n)
{
    PQ *pq = (PQ *)malloc(sizeof(PQ));
    pq->size = 0;
    pq->vec = (void **)malloc(sizeof(void *) * (n + 1));
    return pq;
}

void fix_up(PQ *pq, int (*compare)(const void *, const void *))
{
    // swim up
    int k = pq->size;
    while (k > 1 && compare(pq->vec[k / 2], pq->vec[k]))
    {
        exch(pq->vec[k], pq->vec[k / 2]);
        k = k / 2;
    }
}

void PQ_insert(PQ *pq, void *item, int (*compare)(const void *, const void *))
{
    pq->size++;
    pq->vec[pq->size] = item;
    fix_up(pq, compare);
}

void fix_down(PQ *pq, int (*compare)(const void *, const void *), int k)
{
    while (2 * k <= pq->size)
    {
        int j = 2 * k;
        if (j < pq->size && compare(pq->vec[j], pq->vec[j + 1]))
        {
            j++;
        }
        if (!compare(pq->vec[k], pq->vec[j]))
        {
            break;
        }
        exch(pq->vec[k], pq->vec[j]);
        k = j;
    }
}

void PQ_FixPri(PQ *pq, int (*compare)(const void *, const void *))
{
    fix_down(pq, compare, 1);
}

void *PQ_delmin(PQ *pq, int (*compare)(const void *, const void *))
{
    void *min = pq->vec[1];
    if (pq->size == 0)
    {
        return NULL;
    }
    exch(pq->vec[1], pq->vec[pq->size]);
    pq->size--;
    fix_down(pq, compare, 1);
    return min;
}

void *PQ_min(PQ *pq)
{
    void *min = pq->vec[1];
    if (pq->size == 0)
    {
        return NULL;
    }
    return min;
}

bool PQ_empty(PQ *pq)
{
    return (pq->size == 0);
}

int PQ_size(PQ *pq)
{
    return pq->size;
}

void PQ_finish(PQ *pq)
{
    free(pq->vec);
    free(pq);
}