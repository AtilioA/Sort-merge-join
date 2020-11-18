#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../include/external_sort.h"

void join_lines(char *lineFile1, char *lineFile2, int *fieldsArrayF1, int *fieldsArrayF2, int fieldAmnt, FILE *fileOut)
{
    int i = 0;
    // printf("Join de linhas %s e %s\n", lineFile1, lineFile2);

    int lengthLine1 = count_commas(lineFile1) + 1;
    int lengthLine2 = count_commas(lineFile2) + 1;

    char **line1 = line_to_string_array(lineFile1, lengthLine1);
    char **line2 = line_to_string_array(lineFile2, lengthLine2);

    // Marca campos para join
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
        free(line1[fieldsArrayF1[i]]);
        free(line2[fieldsArrayF2[i]]);
        line1[fieldsArrayF1[i]] = (char *)"*";
        line2[fieldsArrayF2[i]] = (char *)"*";
    }

    // Escreve campos de file1
    for (i = 0; i < lengthLine1; i++)
    {
        if (strcmp(line1[i], "*") != 0)
        {
            fprintf(fileOut, ",%s", line1[i]);
        }
    }

    for (i = 0; i < lengthLine2; i++)
    {
        if (strcmp(line2[i], "*") != 0)
        {
            fprintf(fileOut, ",%s", line2[i]);
        }
    }
    fprintf(fileOut, "\n");

    free_string_array(line1, lengthLine1);
    free_string_array(line2, lengthLine2);
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
        int dataSize1 = count_commas(lineFile1) + 1;
        char **dataLine1 = line_to_string_array(lineFile1, dataSize1);
        Cmp_Data *CmpLine1 = create_cmp_data(dataLine1, fieldsArrayF1, fieldAmnt);

        int dataSize2 = count_commas(lineFile2) + 1;
        char **dataLine2 = line_to_string_array(lineFile2, dataSize2);
        Cmp_Data *CmpLine2 = create_cmp_data(dataLine2, fieldsArrayF2, fieldAmnt);

        compareFields = compare_Cmp_Data(CmpLine1, CmpLine2);
        //printf("cmp:%i\n", compareFields);
        free_Cmp_Data_data(CmpLine1, dataSize1);
        free_Cmp_Data_data(CmpLine2, dataSize2);
        free(CmpLine1);
        free(CmpLine2);
        // Se forem iguais:
        if (compareFields == 0)
        {
            // printf("\nLinhas:\n1: %s2: %s\n\n", lineFile1, lineFile2);

            // Calcular resultado e escrever no arquivo
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
