#ifndef __UTILS_H
#define __UTILS_H

// Biblioteca com funções auxiliares

#include <stdio.h>

// Conta a quantidade de vírgulas em uma string de entrada
int count_commas(char *string);

// Obtém strings de uma linha lida
char **line_to_string_array(char *line, int m);
// Obtém inteiros de uma linha lida
int *line_to_int_array(char *line, int m);
// Libera um vetor de strings da memória
void free_string_array(char **array, int m);

char *join_lines(char *lineFile1, char *lineFile2);
void write_to_file(char *string, FILE *outputFile);
void join_fields(FILE *file1Sorted, FILE *file2Sorted, int *fieldsArrayF1, int *fieldsArrayF2, int fieldAmnt, FILE *fileOut);

#endif
