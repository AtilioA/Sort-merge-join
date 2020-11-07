#ifndef __UTILS_H
#define __UTILS_H

// Biblioteca com funções auxiliares

#include <stdio.h>

// Conta quantas linhas um arquivo de entrada possui
int count_lines(FILE *fpIn);
// Lê e retorna quantas dimensões um arquivo de entrada possui
int determine_dimensions(FILE *fpIn);
char **lineToStringVec(char *line, int m);
int *lineToIntVec(char *line, int m);
void freeStringVec(char **vec, int m);
int read_input_file(FILE *fpIn);
void write_output_file(FILE *fpOut);

#endif
