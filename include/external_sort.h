#ifndef __EXTSORT_H
#define __EXTSORT_H

// Biblioteca para ordenação externa

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "PQ.h"
#include "utils.h"

typedef struct cmp_data Cmp_Data;

/* Ordena um arquivo dados:
    * M: número de registros possíveis de serem armazenados na memória
    * P: número de dispositivos de armazenamento disponíveis
    * fields: os campos a serem analisados para ordenação
    * fieldsAmnt: a quantidade de campos a serem analisados
    * outputName: o nome do arquivo de saída final, ordenado
*/
FILE *sort(FILE *file, int M, int P, int *fields, int fieldsAmnt, const char *outputName);

// Comparador de TAD Cmp_Data
int compare_Cmp_Data(const void *a, const void *b);

// Inicializa um objeto do tipo Cmp_Data
Cmp_Data *create_cmp_data(char **data, int *fieldsToCompare, int fieldAmnt);
// Libera atributo data de um objeto do tipo Cmp_Data
void free_Cmp_Data_data(Cmp_Data *cmpData, int dataSize);

#endif
