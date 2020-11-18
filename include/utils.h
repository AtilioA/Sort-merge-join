#ifndef __UTILS_H
#define __UTILS_H

// Biblioteca com funções auxiliares

// Conta a quantidade de vírgulas em uma string de entrada
int count_commas(char *string);

// Obtém strings de uma linha lida
char **line_to_string_array(char *line, int m);
// Obtém inteiros de uma linha lida
int *line_to_int_array(char *line, int m);
// Libera um vetor de strings da memória
void free_string_array(char **array, int m);

#endif
