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
    char *line2 = malloc(sizeof(char) * (strlen(line) + 1));
    char *temp = line2;
    strncpy(line2, line, (strlen(line) + 1));

    char **array = malloc(sizeof(char *) * m);

    for (int i = 0; i < m; i++)
    {
        // Obtém strings da linha
        char *token = strtok(temp, ",");

        int lenToken = strlen(token);
        array[i] = malloc(sizeof(char) * (lenToken + 1));
        strncpy(array[i], token, (lenToken + 1));

        temp = NULL;
    }

    free(line2);

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

void join_lines(char *lineFile1, char *lineFile2, int *fieldsArrayF1, int *fieldsArrayF2, int fieldAmnt, FILE *fileOut)
{
    int i = 0;
    // printf("Join de linhas %s e %s\n", lineFile1, lineFile2);

    int lengthLine1 = count_commas(lineFile1) + 1;
    int lengthLine2 = count_commas(lineFile2) + 1;

    char **line1 = line_to_string_array(lineFile1, lengthLine1);
    char **line2 = line_to_string_array(lineFile2, lengthLine2);

    // for (i = 0; i < lengthLine1; i++)
    // {
    //     printf("%s\n", line1[i]);
    // }

    for (i = 0; i < fieldAmnt; i++)
    {
        if (i == 0)
        {
            fprintf(fileOut, "%s", line1[fieldsArrayF1[i]]);
        }
        else
        {
            fprintf(fileOut, ",%s", line1[fieldsArrayF1[i]]);
        }

        line1[fieldsArrayF1[i]] = "*";
        line2[fieldsArrayF2[i]] = "*";
    }

    if (strcmp(line1[0], "*") != 0)
    {
        fprintf(fileOut, ",%s", line1[0]);
    }
    for (i = 1; i < lengthLine1; i++)
    {
        if (strcmp(line1[i], "*") != 0)
        {
            fprintf(fileOut, ",%s", line1[i]);
        }
    }

    if (strcmp(line2[0], "*") != 0)
    {
        fprintf(fileOut, ",%s", line2[0]);
    }
    for (i = 1; i < lengthLine2; i++)
    {
        if (strcmp(line2[i], "*") != 0)
        {
            fprintf(fileOut, ",%s", line2[i]);
        }
    }
    fprintf(fileOut, "\n");
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

    // Enquanto nenhum dos dois arquivos tiver acabado:
    while (!feof(file1Sorted) && !feof(file2Sorted))
    {
        if (hasToReadF1)
        {
            // Lê uma linha de file 1
            lineFile1 = NULL;
            n = 0;
            getlineResult1 = getline(&lineFile1, &n, file1Sorted);
            if (!feof(file1Sorted))
            {
                lineFile1[strlen(lineFile1) - 1] = '\0';
            }
        }
        if (hasToReadF2)
        {
            // Lê uma linha de file 2
            lineFile2 = NULL;
            n = 0;
            getlineResult2 = getline(&lineFile2, &n, file2Sorted);
            if (!feof(file2Sorted))
            {
                lineFile2[strlen(lineFile2) - 1] = '\0';
            }
        }

        if (getlineResult1 <= 0 || getlineResult2 <= 0)
        {
            free(lineFile1);
            free(lineFile2);
            return;
        }
        // printf("%s\n", lineFile1);
        // printf("%s\n", lineFile2);

        // printf("%i, %i\n", getlineResult1, getlineResult2);

        /* Lê campos das linhas */
        Cmp_data dataLine1;
        int dataSize1 = count_commas(lineFile1) + 1;
        dataLine1.data = line_to_string_array(lineFile1, dataSize1);
        dataLine1.fieldsToCompare = fieldsArrayF1;
        dataLine1.fieldAmnt = fieldAmnt;

        Cmp_data dataLine2;
        int dataSize2 = count_commas(lineFile2) + 1;
        dataLine2.data = line_to_string_array(lineFile2, dataSize2);
        dataLine2.fieldsToCompare = fieldsArrayF2;
        dataLine2.fieldAmnt = fieldAmnt;

        compareFields = compare_data(&dataLine1, &dataLine2);
        // printf("cmp:%i\n", compareFields);
        free_string_array(dataLine1.data, dataSize1);
        free_string_array(dataLine2.data, dataSize2);

        // Se forem iguais:
        if (compareFields == 0)
        {
            // printf("\nLinhas:\n1: %s2: %s\n\n", lineFile1, lineFile2);

            // Calcular resultado e escrever no arquivo
            // printf("~Faz join~\n");
            join_lines(lineFile1, lineFile2, fieldsArrayF1, fieldsArrayF2, fieldAmnt, fileOut);

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
            free(lineFile2);
            lineFile2 = NULL;
        }
        else // linhaFile1 < linhaFile2
        {
            // Ler próxima linha de file 1
            hasToReadF1 = true;
            hasToReadF2 = false;
            free(lineFile1);
            lineFile1 = NULL;
        }
        n = 0;
    }

    // Finalização
    if (lineFile1 != NULL && hasToReadF1)
    {
        free(lineFile1);
    }
    if (lineFile2 != NULL && hasToReadF2)
    {
        free(lineFile2);
    }
}
