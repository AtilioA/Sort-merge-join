#include "../include/externalSort.h"

typedef struct cmpData
{
    char **data;
    int *columsToCompare;
    int columsSize;
} Cmp_data;
typedef struct pq_item
{
    int deviceIndex;
    int blockSize;
    int actBlockSize;
    Cmp_data data;
} PQ_Item;

int comparePQ_Item(const void *a, const void *b)
{
}

int compareData(const void *a, const void *b)
{
    Cmp_data data1 = *(Cmp_data *)a;
    Cmp_data data2 = *(Cmp_data *)b;
    for (int i = 0; i < data1.columsSize; i++)
    {
        if (strcmp(data1.data[data1.columsToCompare[i]], data2.data[data2.columsToCompare[i]]) != 0)
        {
            return strcmp(data1.data[data1.columsToCompare[i]], data2.data[data2.columsToCompare[i]]);
        }
    }
}

FILE *sort(FILE *file, int M, int P, int *colums, int columsAmnt)
{
    int devAmnt = 2 * P;
    char *line = NULL;
    long unsigned int n = 0;
    getline(&line, &n, file);
    int dataSize = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ',')
        {
            dataSize++;
        }
    }
    dataSize++;
    rewind(file);
    free(line);
    FILE *devs[devAmnt];
    int devNameSize = floor(log10(devAmnt) + log10(fileno(file))) + 7;
    char *devName = malloc(sizeof(char) * devNameSize);
    for (int i = 0; i < devAmnt; i++)
    {
        snprintf(devName, sizeof(char) * devNameSize, "%d_%d.txt", fileno(file), i);
        devs[i] = fopen(devName, "w");
    }
    int N = 0;
    Cmp_data *vec = malloc(sizeof(Cmp_data) * M);
    int fileDest = P;
    while (!feof(file))
    {
        int blockRead = 0;
        for (int i = 0; i < M; i++)
        {
            line = NULL;
            n = 0;
            getline(&line, &n, file);
            if (feof(file))
            {
                break;
            }
            line[strlen(line) - 1] = '\0';
            Cmp_data data;
            data.columsSize = columsAmnt;
            data.columsToCompare = colums;
            data.data = lineToStringVec(line, dataSize);
            free(line);
            for (int j = 0; j < dataSize; j++)
            {
                printf("-%s\n", data.data[j]);
            }
            vec[i] = data;
            blockRead++;
        }
        qsort(vec, blockRead, sizeof(Cmp_data), compareData);
        if (fileDest >= 2 * P)
        {
            fileDest = P;
        }
        for (int i = 0; i < blockRead; i++)
        {
            fprintf(devs[fileDest], "%s", vec[i].data[0]);
            for (int j = 0; j < dataSize; j++)
            {
                fprintf(devs[fileDest], ",%s", vec[i].data[j]);
            }
            fprintf(devs[fileDest], "\n");
        }
        if (feof(file))
        {
            break;
        }
        fileDest++;
    }
}