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

int compare_data(const void *a, const void *b)
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
int compare_PQ_Item(const void *a, const void *b)
{
    PQ_Item *f1 = (PQ_Item *)a;
    PQ_Item *f2 = (PQ_Item *)b;
    if (f1->fileLoop == f2->fileLoop)
    {
        return compare_data(&(f1->data), &(f2->data)) > 0;
    }
    else
    {
        return f1->fileLoop > f2->fileLoop;
    }
}

FILE *sort(FILE *file, int M, int P, int *colums, int columsAmnt, char *outputName)
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
    int devNameSize = floor(log10(devAmnt) + 1) + 5;
    char *devName = malloc(sizeof(char) * devNameSize);
    for (int i = 0; i < devAmnt; i++)
    {
        snprintf(devName, sizeof(char) * devNameSize, "%d.txt", i);
        devs[i] = fopen(devName, "w");
    }
    int N = 0;
    Cmp_data *array = malloc(sizeof(Cmp_data) * M);
    int fileDest = P;
    //Distribuindo os elementos para P até 2P - 1 arquivos
    while (!feof(file))
    {
        int blockRead = 0;
        //Pega bloco de tamanho M do arquivo de entrada
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
            data.data = line_to_string_array(line, dataSize);
            free(line);
            array[i] = data;
            blockRead++;
        }
        //Ordena o bloco
        qsort(array, blockRead, sizeof(Cmp_data), compare_data);
        if (fileDest >= 2 * P)
        {
            fileDest = P;
        }
        //Escreve no dispositivo fileDest
        for (int i = 0; i < blockRead; i++)
        {
            fprintf(devs[fileDest], "%s", array[i].data[0]);
            free(array[i].data[0]);
            for (int j = 1; j < dataSize; j++)
            {
                fprintf(devs[fileDest], ",%s", array[i].data[j]);
                free(array[i].data[j]);
            }
            free(array[i].data);
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
    for (int i = 0; i < k; i++)
    {
        PQ_Item *item;
        //Pega a primeira linha de cada dispositivo de entrada e coloca em uma fila de prioridade
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
            snprintf(devName, sizeof(char) * devNameSize, "%d.txt", j);
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
            item->data.data = line_to_string_array(line, dataSize);
            free(line);
            PQ_insert(priQueue, item, compare_PQ_Item);
        }
        //Reabre os dispositivos de saida como escrita
        for (int j = fileDest; j < fileDest + P; j++)
        {
            fclose(devs[j]);
            snprintf(devName, sizeof(char) * devNameSize, "%d.txt", j);
            devs[j] = fopen(devName, "w");
        }
        int fileLoop = 0;
        while (!PQ_empty(priQueue))
        {
            for (int j = fileDest; j < fileDest + P; j++, fileLoop++)
            {
                //Para cada dispositivo de saida recolhe M*P^k linhas no maximo
                for (int l = 0; l < block * P; l++)
                {
                    item = PQ_del_min(priQueue, compare_PQ_Item);
                    if (item == NULL)
                    {
                        break;
                    }
                    if (item->actBlockSize == 0) //Tamanho maximo do bloco do dispositivo de entrada atingido, próximo dispositivo de saida recebera dessa entrada
                    {
                        item->actBlockSize = item->blockSize;
                        item->fileLoop++;
                        PQ_insert(priQueue, item, compare_PQ_Item);
                        l--;
                        continue;
                    }
                    else
                    {
                        item->actBlockSize--;
                    }
                    if (item->fileLoop != fileLoop) // Não há mais dispositivos para esse dispositivo de saida, vá para o pŕoximo
                    {
                        PQ_insert(priQueue, item, compare_PQ_Item);
                        break;
                    }
                    fprintf(devs[j], "%s", item->data.data[0]);
                    for (int m = 1; m < dataSize; m++)
                    {
                        fprintf(devs[j], ",%s", item->data.data[m]);
                    }
                    fprintf(devs[j], "\n");
                    free_string_array(item->data.data, dataSize);
                    line = NULL;
                    n = 0;
                    getline(&line, &n, devs[item->deviceIndex]);
                    if (feof(devs[item->deviceIndex]) || line == NULL) // Dispositivo de entrada não possui mais dados
                    {
                        if (line != NULL)
                        {
                            free(line);
                        }
                        free(item);
                        l--;
                        continue;
                    }
                    line[strlen(line) - 1] = '\0';
                    item->data.data = line_to_string_array(line, dataSize);
                    free(line);
                    PQ_insert(priQueue, item, compare_PQ_Item);
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
    //printf("%d\n", fileSrc);
    snprintf(devName, sizeof(char) * devNameSize, "%d.txt", fileSrc);
    rename(devName, outputName);
    PQ_finish(priQueue);
    free(devName);
    free(array);
    return fopen(outputName, "r");
}
