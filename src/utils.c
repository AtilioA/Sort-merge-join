#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int count_commas(char *string)
{
    int commaAmnt = 0;
    int len = strlen(string);
    for (int i = 0; i < len; i++)
    {
        if (string[i] == ',')
        {
            commaAmnt++;
        }
    }

    return commaAmnt;
}

int count_lines(FILE *fpIn)
{
    char c;
    int lines = 0;

    if (fpIn == NULL)
    {
        printf("Não foi possivel abrir o arquivo.");
        return -1;
    }

    // Percorre arquivo contando \n (que finaliza uma linha)
    for (c = getc(fpIn); c != EOF; c = getc(fpIn))
    {
        if (c == '\n')
        {
            lines++;
        }
    }

    return lines;
}

int read_input_file(FILE *fpIn, char **names, double *coordenates)
{
    // Inicializa variáveis para leitura de linhas
    char *lineBuffer = NULL;
    size_t lineBufferSize = 0;
    ssize_t currentLine;

    // Lê primeira linha do arquivo
    currentLine = getline(&lineBuffer, &lineBufferSize, fpIn);
    int nNames = 0;
    int nCoordenates = 0;
    int isName = 1;
    // Lê enquanto getline não retornar -1 para currentLine
    while (currentLine >= 0)
    {
        // Printa linha atual
        const char s[2] = ",";

        // A primeira entrada é uma cadeira de caracteres
        isName = 1;

        char *token;
        token = strtok(lineBuffer, s);

        // Percorre até o final da linha
        while (token != NULL)
        {
            if (isName == 1)
            {
                names[nNames] = malloc((strlen(token) + 1) * sizeof(char));
                strcpy(names[nNames], token);
                isName = 0;
                nNames++;
            }
            else
            {
                char *ptr;
                coordenates[nCoordenates] = strtod(token, &ptr);
                nCoordenates++;
            }

            // Lê próximo token da linha
            token = strtok(NULL, s);
        }
        // printf("chars=%06zd, buf size=%06zu, contents: %s", currentLine, lineBufferSize, lineBuffer);

        // Lê a próxima linha
        currentLine = getline(&lineBuffer, &lineBufferSize, fpIn);
    }
    free(lineBuffer);
    lineBuffer = NULL;

    return 0;
}

/*
* Retorna vetor de strings
* Entrada: String com valores separados por virgula, tamanho do vetor
* Saida: Vetor com os valores
*/
char **line_to_string_array(char *line, int m)
{
    char *line2 = malloc(sizeof(char) * (strlen(line) + 1));
    char *temp = line2;
    strncpy(line2, line, (strlen(line) + 1));
    char **array = malloc(sizeof(char *) * m);
    for (int i = 0; i < m; i++)
    {
        char *token = strtok(temp, ",");
        array[i] = malloc(sizeof(char) * (strlen(token) + 1));
        strncpy(array[i], token, (strlen(token) + 1));
        temp = NULL;
    }
    free(line2);
    return array;
}

int *line_to_int_array(char *line, int m)
{
    char *line2 = malloc(sizeof(char) * (strlen(line) + 1));
    char *temp = line2;
    strncpy(line2, line, (strlen(line) + 1));
    int *array = malloc(sizeof(int) * m);
    for (int i = 0; i < m; i++)
    {
        char *token = strtok(temp, ",");
        array[i] = atoi(token);
        temp = NULL;
    }
    free(line2);
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

void write_output_file(FILE *fpOut)
{
}
