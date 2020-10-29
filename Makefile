
OBJ      := obj
SRC      := src
INC      := include
CFLAGS   := -lm -g -O2 -Wall -pedantic -Wextra -Wwrite-strings
EXE 	 := trab2
PROJETO  := main

# Cria objetos de todos os arquivos de código-fonte para então linká-los no programa final
main: clean $(OBJ)/utils.o $(OBJ)/$(PROJETO).o
	gcc $(OBJ)/*.o -o $(EXE) $(CFLAGS)

$(OBJ)/utils.o: $(SRC)/utils.c $(INC)/utils.h
	gcc -c $(CFLAGS) "$(SRC)/utils.c" -o "$(OBJ)/utils.o"

$(OBJ)/$(PROJETO).o: $(PROJETO).c
	gcc -c $(CFLAGS) $(PROJETO).c -o "$(OBJ)/$(PROJETO).o"

# Roda o programa com entrada 1
# run_1: clean main
# 	./$(EXE) entradas/1.txt 2 saidas/saida_1.txt

# # Roda o programa com entrada 2
# run_2: clean main
# 	./$(EXE) entradas/2.txt 4 saidas/saida_2.txt


# # Roda o programa com entrada 3
# run_3: clean main
# 	./$(EXE) entradas/3.txt 5 saidas/saida_3.txt


# # Roda o programa com entrada 4
# run_4: clean main
# 	./$(EXE) entradas/4.txt 5 saidas/saida_4.txt


# # Roda o programa com entrada 5
# run_5: clean main
# 	./$(EXE) entradas/5.txt 10 saidas/saida_5.txt

# Roda o programa com valgrind e flags úteis (entrada 3)
# val: main
# 	valgrind --leak-check=full -v --track-origins=yes \
# 	--show-leak-kinds=all ./$(EXE) entradas/3.txt 5 saidas/saida.txt

# # Roda o programa com valgrind sem flags (entrada 3)
# valzin: main
# 	valgrind ./$(EXE) entradas/3.txt 5 saidas/saida_3.txt

# # Roda o programa com valgrind e salva resultados num .txt (entrada 3)
# valtxt: main
# 	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes --verbose --log-file="valgrind-out.txt" ./$(EXE) entradas/3.txt 5 saidas/saida_3.txt

# Limpa objetos e o executável do programa
clean:
	-rm $(OBJ)/*.o
	-rm $(EXE)
