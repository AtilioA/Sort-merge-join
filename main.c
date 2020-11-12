
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/utils.h"
#include "include/external_sort.h"

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
    FILE *file2 = fopen(fileIn2Name, "r");
    FILE *fileOut = fopen(fileOutName, "w");

    FILE *file1Sort = sort(file1, M, P, fieldsArrayF1, commaAmnt + 1, "sorted1.txt");
    FILE *file2Sort = sort(file2, M, P, fieldsArrayF2, commaAmnt + 1, "sorted2.txt");

    free(fieldsArrayF1);
    free(fieldsArrayF2);

    fclose(file1);
    fclose(file2);

    join_fields(file1Sort, file2Sort, fieldsArrayF1, fieldsArrayF2, fileOut);

    fclose(fileOut);
    fclose(file1Sort);
    fclose(file2Sort);

    return EXIT_SUCCESS;
}
