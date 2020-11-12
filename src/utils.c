#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

void join_fields(FILE *file1Sorted, FILE *file2Sorted, FILE *fileOut)
{
    // Inicialização
    ssize_t getlineResult = 0;
    char *lineFile1 = NULL;
    char *lineFile2 = NULL;
    long unsigned int n = 0;

    int compareStrings = 0;

    bool hasToReadF1 = true;
    bool hasToReadF2 = true;

    char *joinResult = NULL;

    // Enquanto nenhum dos dois arquivos tiver acabado:
    while (!feof(file1Sorted) && !feof(file2Sorted))
    {
        if (hasToReadF1)
        {
            // Lê uma linha de file 1
            getlineResult = getline(&lineFile1, &n, file1Sorted);
        }
        if (hasToReadF2)
        {
            // Lê uma linha de file 2
            getlineResult = getline(&lineFile2, &n, file2Sorted);
        }

        // Se forem iguais:
        compareStrings = strcmp(lineFile1, lineFile2);
        if (compareStrings == 0)
        {
            // Calcular resultado e escrever no arquivo
            joinResult = join_lines(lineFile1, lineFile2);
            write_to_file(joinResult, fileOut);

            // Ler próxima linha de file 1 e de file 2
            hasToReadF1 = true;
            hasToReadF2 = true;
        }
        // Se linhaFile1 > linhaFile2:
        if (compareStrings >= 1)
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
    // ?
}
