#ifndef __JOIN_H
#define __JOIN_H

// Biblioteca com funções para realização de join de arquivos

// Realiza join de linhas de dois arquivos de acordo com seus campos de interesse
void join_lines(char *lineFile1, char *lineFile2, int *fieldsArrayF1, int *fieldsArrayF2, int fieldAmnt, FILE *fileOut);

// Realiza join de dois arquivos
void join_fields(FILE *file1Sorted, FILE *file2Sorted, int *fieldsArrayF1, int *fieldsArrayF2, int fieldAmnt, FILE *fileOut);

#endif
