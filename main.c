
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/utils.h"
#include "include/externalSort.h"

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
        if (iFields[i] == ',')
        {
            commaAmnt++;
        }
    }
    int *vec = lineToIntVec(iFields, commaAmnt + 1);
    // Guarda nome do arquivo de entrada dos argumentos
    char *fileIn1 = argv[5];
    char *fileIn2 = argv[6];
    FILE *file1 = fopen(fileIn1, "r");
    FILE *file2 = fopen(fileIn2, "r");
    // Guarda nome do arquivo de saída
    FILE *teste = sort(file1, M, P, vec, commaAmnt + 1);
    char *fileOut = argv[7];

    free(vec);
    fclose(file1);
    fclose(file2);

    return EXIT_SUCCESS;
}
