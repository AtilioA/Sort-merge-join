#include "../include/externalSort.h"
#include <unistd.h>
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
    int fileLoop;
    Cmp_data data;
} PQ_Item;

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
    return 0;
}
int comparePQ_Item(const void *a, const void *b)
{
    PQ_Item *f1 = (PQ_Item *)a;
    PQ_Item *f2 = (PQ_Item *)b;
    if (f1->fileLoop == f2->fileLoop)
    {
        return compareData(&(f1->data), &(f2->data)) > 0;
    }
    else
    {
        return f1->fileLoop > f2->fileLoop;
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
    int devNameSize = floor(log10(devAmnt) + log10(fileno(file))) + 8;
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
                free(line);
                break;
            }
            N++;
            line[strlen(line) - 1] = '\0';
            Cmp_data data;
            data.columsSize = columsAmnt;
            data.columsToCompare = colums;
            data.data = lineToStringVec(line, dataSize);
            free(line);
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
            free(vec[i].data[0]);
            for (int j = 1; j < dataSize; j++)
            {
                fprintf(devs[fileDest], ",%s", vec[i].data[j]);
                free(vec[i].data[j]);
            }
            free(vec[i].data);
            fprintf(devs[fileDest], "\n");
        }
        if (feof(file))
        {
            break;
        }
        fileDest++;
    }
    fileDest = 0;
    int fileSrc = P;
    int block = M;
    int k = ceil(log(N / M) / log(P)) + 1;
    PQ *priQueue = PQ_init(P);
    //printf("%d %d %d %d\n", N, M, P, k);

    for (int i = 0; i < k; i++)
    {
        PQ_Item *item;
        for (int j = fileSrc; j < fileSrc + P; j++)
        {
            item = malloc(sizeof(PQ_Item));
            item->deviceIndex = j;
            item->blockSize = block;
            item->actBlockSize = block;
            item->fileLoop = 0;
            item->data.columsToCompare = colums;
            item->data.columsSize = columsAmnt;
            fclose(devs[j]);
            snprintf(devName, sizeof(char) * devNameSize, "%d_%d.txt", fileno(file), j);
            devs[j] = fopen(devName, "r");
            line = NULL;
            n = 0;
            getline(&line, &n, devs[j]);
            if (feof(devs[j]))
            {
                free(line);
                free(item);
                continue;
            }
            line[strlen(line) - 1] = '\0';
            item->data.data = lineToStringVec(line, dataSize);
            free(line);
            PQ_insert(priQueue, item, comparePQ_Item);
        }
        for (int j = fileDest; j < fileDest + P; j++)
        {
            fclose(devs[j]);
            snprintf(devName, sizeof(char) * devNameSize, "%d_%d.txt", fileno(file), j);
            devs[j] = fopen(devName, "w");
        }
        int fileLoop = 0;
        while (!PQ_empty(priQueue))
        {
            for (int j = fileDest; j < fileDest + P; j++, fileLoop++)
            {
                for (int l = 0; l < block * P; l++)
                {
                    //printf("/%d %d", PQ_size(priQueue), PQ_empty(priQueue));
                    item = PQ_delmin(priQueue, comparePQ_Item);
                    if (item == NULL)
                    {
                        break;
                    }
                    //printf(" %s %d %d %d bf ", item->data.data[0], item->fileLoop, fileLoop, item->actBlockSize);
                    if (item->actBlockSize == 0)
                    {
                        item->actBlockSize = item->blockSize;
                        item->fileLoop++;
                        PQ_insert(priQueue, item, comparePQ_Item);
                        l--;
                        continue;
                    }
                    else
                    {
                        item->actBlockSize--;
                    }
                    if (item->fileLoop != fileLoop)
                    {
                        PQ_insert(priQueue, item, comparePQ_Item);
                        break;
                    }
                    //printf("af %s %d %d\\\n", item->data.data[0], item->fileLoop, fileLoop);
                    fprintf(devs[j], "%s", item->data.data[0]);
                    //printf("%s", item->data.data[0]);
                    for (int m = 1; m < dataSize; m++)
                    {
                        //printf(",%s", item->data.data[m]);
                        fprintf(devs[j], ",%s", item->data.data[m]);
                    }
                    //printf("\n");
                    fprintf(devs[j], "\n");
                    //printf("%d %d\n", j, l);
                    freeStringVec(item->data.data, dataSize);
                    line = NULL;
                    n = 0;
                    getline(&line, &n, devs[item->deviceIndex]);
                    if (feof(devs[item->deviceIndex]) || line == NULL)
                    {
                        if (line != NULL)
                        {
                            free(line);
                        }
                        //printf("aaa %d\n", item->deviceIndex);
                        free(item);
                        l--;
                        continue;
                    }
                    line[strlen(line) - 1] = '\0';
                    //printf("%s-a\n", line);
                    item->data.data = lineToStringVec(line, dataSize);
                    free(line);
                    PQ_insert(priQueue, item, comparePQ_Item);
                }
            }
        }
        if (fileDest == P)
        {
            fileDest = 0;
        }
        else
        {
            fileDest = P;
        }
        if (fileSrc == P)
        {
            fileSrc = 0;
        }
        else
        {
            fileSrc = P;
        }
        block *= P;
    }
    for (int i = 0; i < devAmnt; i++)
    {
        fclose(devs[i]);
    }
    PQ_finish(priQueue);
    free(devName);
    free(vec);
}