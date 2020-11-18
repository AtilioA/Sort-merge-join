#include <stdlib.h>
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
        if (strcmp(array[i], "*") != 0)
        {
            free(array[i]);
        }
    }
    free(array);
}
