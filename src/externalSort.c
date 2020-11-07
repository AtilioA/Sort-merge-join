#include "../include/externalSort.h"

typedef struct pq_item
{
    int deviceIndex;
    int blockSize;
    int actBlockSize;
    char *data;
    int *columsToCompare;
    int columsSize;
} PQ_Item;

FILE *sort(FILE *file, int M, int P, int *colums, int columsAmnt)
{
}