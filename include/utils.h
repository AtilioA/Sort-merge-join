#ifndef __UTILS_H
#define __UTILS_H

// Biblioteca com funções auxiliares

#include <stdio.h>

// Conta a quantidade de vírgulas em uma string de entrada
int count_commas(char *string);
// Conta quantas linhas um arquivo de entrada possui
int count_lines(FILE *fpIn);
// Lê e retorna quantas dimensões um arquivo de entrada possui
int determine_dimensions(FILE *fpIn);
char **line_to_string_array(char *line, int m);
int *line_to_int_array(char *line, int m);
void free_string_array(char **array, int m);
int read_input_file(FILE *fpIn);
void write_output_file(FILE *fpOut);

#endif
