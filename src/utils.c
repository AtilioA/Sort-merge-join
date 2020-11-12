#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../include/external_sort.h"

typedef struct cmpData
{
    char **data;
    int *fieldsToCompare;
    int fieldAmnt;
} Cmp_data;

int count_commas(char *string)
{
    int commaAmnt = 0;
    for (unsigned int i = 0; i < strlen(string); i++)
    {
        if (string[i] == ',')
        {
            commaAmnt++;
        }
    }

    return commaAmnt;
}

char **line_to_string_array(char *line, int m)
{
    // char *line2 = malloc(sizeof(char) * (strlen(line) + 1));
    // strncpy(line2, line, (strlen(line) + 1));

    char **array = malloc(sizeof(char *) * m);

    for (int i = 0; i < m; i++)
    {
        // Obtém strings da linha
        char *token = strtok(line, ",");

        int lenToken = strlen(token);
        array[i] = malloc(sizeof(char) * (lenToken + 1));
        strncpy(array[i], token, (lenToken + 1));

        line = NULL;
    }

    // free(line2);

    return array;
}

int *line_to_int_array(char *line, int m)
{
    // char *line2 = malloc(sizeof(char) * (strlen(line) + 1));
    // char *temp = line;
    // strncpy(line2, line, (strlen(line) + 1));

    int *array = malloc(sizeof(int) * m);

    for (int i = 0; i < m; i++)
    {
        // Obtém inteiros da linha
        char *token = strtok(line, ",");
        array[i] = atoi(token);
        line = NULL;
    }

    // free(line2);

    return array;
}

void free_string_array(char **array, int m)
{
    for (int i = 0; i < m; i++)
    {
        free(array[i]);
    }
    free(array);
}

char *join_lines(char *lineFile1, char *lineFile2)
{
    return lineFile1;
}
void write_to_file(char *string, FILE *outputFile)
{
}

void join_fields(FILE *file1Sorted, FILE *file2Sorted, int *fieldsArrayF1, int *fieldsArrayF2, int fieldAmnt, FILE *fileOut)
{
    // Inicialização
    ssize_t getlineResult1 = 1;
    ssize_t getlineResult2 = 1;
    char *lineFile1 = NULL;
    char *lineFile2 = NULL;
    long unsigned int n = 0;

    int compareFields = 0;

    bool hasToReadF1 = true;
    bool hasToReadF2 = true;

    char *joinResult = NULL;

    // printf("%i\n%i\n%i", !feof(file1Sorted), !feof(file1Sorted), getlineResult1);

    // Enquanto nenhum dos dois arquivos tiver acabado:
    while (!feof(file1Sorted) && !feof(file2Sorted))
    {
        if (hasToReadF1)
        {
            // Lê uma linha de file 1
            getlineResult1 = getline(&lineFile1, &n, file1Sorted);
        }
        if (hasToReadF2)
        {
            // Lê uma linha de file 2
            getlineResult2 = getline(&lineFile2, &n, file2Sorted);
            printf("2: %i\n", getlineResult2);
        }

        if (getlineResult1 <= 0 || getlineResult2 <= 0)
        {
            return;
        }
        printf("%s", lineFile1);
        printf("%s", lineFile2);

        // printf("%i, %i\n", getlineResult1, getlineResult2);

        /* Lê campos das linhas */
        Cmp_data dataLine1;
        dataLine1.data = lineFile1;
        dataLine1.fieldsToCompare = line_to_int_array(fieldsArrayF1, fieldAmnt);
        dataLine1.fieldAmnt = fieldAmnt;
        Cmp_data dataLine2;
        dataLine1.data = lineFile2;
        dataLine1.fieldsToCompare = line_to_int_array(fieldsArrayF2, fieldAmnt);
        dataLine1.fieldAmnt = fieldAmnt;

        compareFields = compare_data(dataLine1.data, dataLine2.data);
        printf("cmp:%i\n", compareFields);

        // Se forem iguais:
        if (compareFields == 0)
        {
            // printf("\nLinhas:\n1: %s2: %s\n\n", lineFile1, lineFile2);

            // Calcular resultado e escrever no arquivo
            printf("~Faz join~\n");
            joinResult = join_lines(lineFile1, lineFile2);
            printf("~Escreve~\n\n");
            write_to_file(joinResult, fileOut);

            // Ler próxima linha de file 1 e de file 2
            hasToReadF1 = true;
            hasToReadF2 = true;
        }
        // Se linhaFile1 > linhaFile2:
        if (compareFields >= 1)
        {
            // Ler próxima linha de file 2
            hasToReadF1 = false;
            hasToReadF2 = true;
        }
        else // linhaFile1 < linhaFile2
        {
            // Ler próxima linha de file 1
            hasToReadF1 = true;
            hasToReadF2 = false;
        }
    }

    // Finalização
    // if (lineFile1 != NULL /*&& hasToReadF1*/)
    // {
    //     free(lineFile1);
    // }
    // if (lineFile2 != NULL && /*hasToReadF2*/)
    // {
    //     free(lineFile2);
    // }
}
