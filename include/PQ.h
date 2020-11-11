#ifndef __PQ_H
#define __PQ_H

// Tipo opaco para priority queue (heap)

#include <stdbool.h>
#include <stdlib.h>

#define exch(A, B)   \
    {                \
        void *t = A; \
        A = B;       \
        B = t;       \
    }

typedef struct pq PQ;

// Inicializa uma estrutura priority queue
PQ *PQ_init(int n);

// Insere um elemento à priority queue
void PQ_insert(PQ *pq, void *item, int (*compare)(const void *, const void *));

// Deleta o menor elemento da priority queue
void *PQ_del_min(PQ *pq, int (*compare)(const void *, const void *));

// Encontra o menor elemento da priority queue
void *PQ_min(PQ *pq);

// Realiza fix-down no primeiro elemento da priority queue
void PQ_fix_first(PQ *pq, int (*compare)(const void *, const void *));

// Verifica se a priority queue está vazia
bool PQ_empty(PQ *pq);

// Retorna o tamanho da priority queue
int PQ_size(PQ *pq);

// Libera a priority queue da memória
void PQ_free(PQ *pq);

#endif
