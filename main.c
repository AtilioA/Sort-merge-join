
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/utils.h"

int main(int argc, char *argv[])
{
    if (argc < 8)
    {
        return EXIT_FAILURE;
    }
    int P = atoi(argv[1]);
    int M = atoi(argv[2]);
    char *iFields = argv[3];
    char *jFields = argv[4];
    int commaAmnt = 0;
    for (int i = 0; i < strlen(iFields); i++)
    {
        if (i[iFields] == ',')
        {
            commaAmnt++;
        }
    }
    int *vec = lineToIntVec(iFields, commaAmnt + 1);
    for (int i = 0; i <= commaAmnt; i++)
    {
        printf("%d\n", vec[i]);
    }
    free(vec);
    // Guarda nome do arquivo de entrada dos argumentos
    char *fileIn1 = argv[5];
    char *fileIn2 = argv[6];
    FILE *file1 = fopen(fileIn1, "r");
    FILE *file2 = fopen(fileIn2, "r");
    // Guarda nome do arquivo de saída
    char *fileOut = argv[7];

    fclose(file1);
    fclose(file2);

    return EXIT_SUCCESS;
}
