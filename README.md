# Sort-merge-join

🗃️ Implementação de um algoritmo de ordenação externa em C que computa o join de campos de dois arquivos de texto, apresentado como requisito parcial para aprovação na disciplina de Estrutura de Dados II pela Universidade Federal do Espírito Santo.

Foi utilizada a linguagem C e implementada a estrutura de [fila de prioridade](https://en.wikipedia.org/wiki/Priority_queue) [(com heaps)](https://en.wikipedia.org/wiki/Priority_queue#Usual_implementation), dentre algumas outras.

# Utilização
O programa é compatível com plataformas UNIX; a compatibilidade com Windows não é o foco deste projeto.

Um executável pode ser obtido chamando [`make`](https://linux.die.net/man/1/make) dentro do diretório do projeto. A execução do programa se dá por 4 argumentos:

1. `./trab2`: o executável;

2. `P`: a quantidade de dispositivos que poderão ser usados para cada arquivo de entrada;

3. `M`: a quantidade de registros que podem ser carregados na memória de uma só vez;

4. `x1,y1,z1...`: os campos do primeiro arquivo a serem utilizados para ordenação e realização do join entre arquivos;

5. `x2,y2,z2...`: os campos do segundo arquivo a serem utilizados para ordenação e realização do join entre arquivos;

6. `file1.txt`: o primeiro arquivo de entrada para leitura;

7. `file2.txt`: o segundo arquivo de entrada para leitura;

8. `fileOut.txt`: o arquivo de saída onde será escrito o resultado do join.

Ex: `./trab2 3 1000000 0,1,4 4,0,1 input/1/file1.txt input/1/file2.txt output/file_1.txt`
