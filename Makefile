ex03: main.o word_counter.o
	gcc main.o word_counter.o -o ex03
main.o: main.c
	gcc -Wall -Wextra -Wpedantic -std=c11 -c main.c -o main.o
word_counter.o: word_counter.c
	gcc -Wall -Wextra -Wpedantic -std=c11 -c word_counter.c -o word_counter.o
clear:
	rm ex03 *.o
