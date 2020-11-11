#include "../include/external_sort.h"
#include <unistd.h>

typedef struct cmpData
{
    char **data;
    int *fieldsToCompare;
    int fieldsSize;
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
    Cmp_data dataFile1 = *(Cmp_data *)a;
    Cmp_data dataFile2 = *(Cmp_data *)b;

    // Retorna a primeira ocorrência em que os dois campos são iguais
    for (int i = 0; i < dataFile1.fieldsSize; i++)
    {
        if (strcmp(dataFile1.data[dataFile1.fieldsToCompare[i]], dataFile2.data[dataFile2.fieldsToCompare[i]]) != 0)
        {
            return strcmp(dataFile1.data[dataFile1.fieldsToCompare[i]], dataFile2.data[dataFile2.fieldsToCompare[i]]);
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

FILE *sort(FILE *file, int M, int P, int *fields, int fieldsAmnt, const char *outputName)
{
    int deviceAmnt = 2 * P;
    char *line = NULL;
    long unsigned int n = 0;
    ssize_t getlineResult = 0;

    // Atribui nomes aos dispositivos e cria arquivos
    FILE *devs[deviceAmnt];
    int deviceNameSize = floor(log10(deviceAmnt) + 1) + 5;
    char *deviceName = malloc(sizeof(char) * deviceNameSize);
    for (int i = 0; i < deviceAmnt; i++)
    {
        snprintf(deviceName, sizeof(char) * deviceNameSize, "%d.txt", i);
        devs[i] = fopen(deviceName, "w");
    }

    int N = 0;
    Cmp_data *array = malloc(sizeof(Cmp_data) * M);
    int fileDest = P;

    // Conta tamanho dos dados
    getlineResult = getline(&line, &n, file);
    if (getlineResult <= 0)
    {
        return NULL;
    }
    // Por que + 1?
    int dataSize = count_commas(line) + 1;
    rewind(file);
    free(line);

    // Distribui elementos para P até 2P - 1 arquivos
    while (!feof(file))
    {
        int blockRead = 0;
        // Pega bloco de tamanho M do arquivo de entrada
        for (int i = 0; i < M; i++)
        {
            line = NULL;
            n = 0;

            getlineResult = getline(&line, &n, file);
            if (feof(file))
            {
                free(line);
                break;
            }

            N++;
            line[strlen(line) - 1] = '\0';

            Cmp_data data;
            data.fieldsSize = fieldsAmnt;
            data.fieldsToCompare = fields;
            data.data = line_to_string_array(line, dataSize);

            free(line);
            array[i] = data;
            blockRead++;
        }

        // Ordena o bloco
        qsort(array, blockRead, sizeof(Cmp_data), compare_data);

        if (fileDest >= 2 * P)
        {
            fileDest = P;
        }

        // Escreve no dispositivo fileDest
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
        // Pega a primeira linha de cada dispositivo de entrada e coloca em uma fila de prioridade
        for (int j = fileSrc; j < fileSrc + P; j++)
        {
            item = malloc(sizeof(PQ_Item));
            item->deviceIndex = j;
            item->blockSize = block;
            item->actBlockSize = block;
            item->fileLoop = 0;
            item->data.fieldsToCompare = fields;
            item->data.fieldsSize = fieldsAmnt;

            fclose(devs[j]);
            snprintf(deviceName, sizeof(char) * deviceNameSize, "%d.txt", j);
            devs[j] = fopen(deviceName, "r");

            line = NULL;
            n = 0;

            getlineResult = getline(&line, &n, devs[j]);
            if (feof(devs[j]))
            {
                free(line);
                free(item);
                continue;
            }

            line[strlen(line) - 1] = '\0';
            item->data.data = line_to_string_array(line, dataSize);
            PQ_insert(priQueue, item, compare_PQ_Item);

            free(line);
        }
        // Reabre os dispositivos de saída como escrita
        for (int j = fileDest; j < fileDest + P; j++)
        {
            fclose(devs[j]);
            snprintf(deviceName, sizeof(char) * deviceNameSize, "%d.txt", j);
            devs[j] = fopen(deviceName, "w");
        }
        int fileLoop = 0;
        while (!PQ_empty(priQueue))
        {
            for (int j = fileDest; j < fileDest + P; j++, fileLoop++)
            {
                // Para cada dispositivo de saída ,recolhe M*P^k linhas no máximo
                for (int l = 0; l < block * P; l++)
                {
                    item = PQ_del_min(priQueue, compare_PQ_Item);
                    if (item == NULL) // heap vazia?
                    {
                        break;
                    }

                    if (item->actBlockSize == 0) // Tamanho máximo do bloco do dispositivo de entrada atingido, próximo dispositivo de saída receberá dessa entrada
                    {
                        item->actBlockSize = item->blockSize;
                        item->fileLoop++;

                        PQ_insert(priQueue, item, compare_PQ_Item);

                        l--;

                        continue;
                    }
                    else // Bloco ainda comporta entrada
                    {
                        item->actBlockSize--;
                    }

                    if (item->fileLoop != fileLoop) // Não há mais dispositivos para esse dispositivo de saída, vá para o pŕoximo
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

                    getlineResult = getline(&line, &n, devs[item->deviceIndex]);
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

                    PQ_insert(priQueue, item, compare_PQ_Item);

                    free(line);
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
    for (int i = 0; i < deviceAmnt; i++)
    {
        fclose(devs[i]);
    }

    snprintf(deviceName, sizeof(char) * deviceNameSize, "%d.txt", fileSrc);
    rename(deviceName, outputName);

    // Libera estruturas auxiliares
    PQ_free(priQueue);
    free(deviceName);
    free(array);

    return fopen(outputName, "r");
}
