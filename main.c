
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/utils.h"
#include "include/external_sort.h"
#include "include/join.h"

int main(int argc, char *argv[])
{
    if (argc < 8)
    {
        return EXIT_FAILURE;
    }

    // Guarda a quantidade de dispositivos disponíveis
    int P = atoi(argv[1]);
    // Guarda a quantidade de registros que cabem na memória principal
    int M = atoi(argv[2]);
    // Guarda os campos a serem ordenados no primeiro arquivo
    char *file1Fields = argv[3];
    // Guarda os campos a serem ordenados no segundo arquivo
    char *file2Fields = argv[4];
    // Guarda nome dos arquivos de entrada
    char *fileIn1Name = argv[5];
    char *fileIn2Name = argv[6];
    // Guarda nome do arquivo de saída
    char *fileOutName = argv[7];

    // Conta quantidade de vírgulas (ambos os arquivos devem receber a mesma quantidade de campos a serem ordenados)
    int commaAmnt = count_commas(file1Fields);

    // Transforma entrada de campos em um vetor de inteiros
    int *fieldsArrayF1 = line_to_int_array(file1Fields, commaAmnt + 1);
    int *fieldsArrayF2 = line_to_int_array(file2Fields, commaAmnt + 1);

    FILE *file1 = fopen(fileIn1Name, "r");
    if (file1 == NULL)
    {
        printf("Não foi possível abrir %s", fileIn1Name);
        return EXIT_FAILURE;
    }
    FILE *file2 = fopen(fileIn2Name, "r");
    if (file2 == NULL)
    {
        printf("Não foi possível abrir %s", fileIn2Name);
        return EXIT_FAILURE;
    }
    FILE *fileOut = fopen(fileOutName, "w");

    FILE *file1Sort = sort(file1, M, P, fieldsArrayF1, commaAmnt + 1, "sorted1.txt");
    FILE *file2Sort = sort(file2, M, P, fieldsArrayF2, commaAmnt + 1, "sorted2.txt");

    fclose(file1);
    fclose(file2);

    fclose(file1Sort);
    fclose(file2Sort);

    file1Sort = fopen("sorted1.txt", "r");
    file2Sort = fopen("sorted2.txt", "r");

    join_fields(file1Sort, file2Sort, fieldsArrayF1, fieldsArrayF2, commaAmnt + 1, fileOut);

    free(fieldsArrayF1);
    free(fieldsArrayF2);
    fclose(file1Sort);
    fclose(file2Sort);
    fclose(fileOut);

    return EXIT_SUCCESS;
}
