
OBJ      := obj
SRC      := src
INC      := include
CFLAGS   := -lm -g -O2 -Wall -pedantic -Wextra -Wwrite-strings -Wno-stringop-overflow
EXE 	 := trab2
PROJETO  := main

# Cria objetos de todos os arquivos de código-fonte para então linká-los no programa final
main: clean $(OBJ)/join.o $(OBJ)/utils.o $(OBJ)/PQ.o $(OBJ)/external_sort.o $(OBJ)/$(PROJETO).o
	gcc $(OBJ)/*.o -o $(EXE) $(CFLAGS)

$(OBJ)/join.o: $(SRC)/join.c $(INC)/join.h
	gcc -c $(CFLAGS) "$(SRC)/join.c" -o "$(OBJ)/join.o"

$(OBJ)/utils.o: $(SRC)/utils.c $(INC)/utils.h
	gcc -c $(CFLAGS) "$(SRC)/utils.c" -o "$(OBJ)/utils.o"

$(OBJ)/PQ.o: $(SRC)/PQ.c $(INC)/PQ.h
	gcc -c $(CFLAGS) "$(SRC)/PQ.c" -o "$(OBJ)/PQ.o"

$(OBJ)/external_sort.o: $(SRC)/external_sort.c $(INC)/external_sort.h
	gcc -c $(CFLAGS) "$(SRC)/external_sort.c" -o "$(OBJ)/external_sort.o"

$(OBJ)/$(PROJETO).o: $(PROJETO).c
	gcc -c $(CFLAGS) $(PROJETO).c -o "$(OBJ)/$(PROJETO).o"

# Roda o programa com exemplo da especificação
run_ex: clean main
	./$(EXE) 3 1000000 1,0 0,2 input/ex1.txt input/ex2.txt output/fileOutEx.txt
	code -r output/fileOutEx.txt

#  Roda o programa com entrada 1
run_1: clean main
	./$(EXE) 3 1000000 0,1,4 4,0,1 input/1/file1.txt input/1/file2.txt output/file_1.txt
	code -r output/file_1.txt

#  Roda o programa com entrada 2
run_2: clean main
	./$(EXE) 3 1000000 0,1,4 4,0,1 input/2/file1.txt input/2/file2.txt output/file_2.txt
	code -r output/file_2.txt

# Roda o programa com valgrind e flags úteis (entrada 3)
val: main
	valgrind --leak-check=full -v --track-origins=yes \
	--show-leak-kinds=all ./$(EXE) 3 1000000 0,1,4 4,0,1 input/1/file1.txt input/1/file2.txt output/fileOut2.txt

# Roda o programa com valgrind sem flags (entrada 3)
valzin: main
	valgrind ./$(EXE) input/3.txt 5 output/saida_3.txt


# Roda o programa com valgrind e salva resultados num .txt (entrada 3)
valtxt: main
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes --verbose --log-file="valgrind-out.txt" ./$(EXE) 3 1000000 1,0 0,2 input/file1.txt input/file2.txt output/fileOut2.txt

# Limpa objetos e o executável do programa
clean: #cleantxt
	-rm $(OBJ)/*.o
	-rm $(EXE)

cleantxt:
	-rm *.txt
