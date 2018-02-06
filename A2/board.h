#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


extern char Die[16][6];

typedef struct cell {
	char letter;
	int visited;
	int num_ngbs;
	struct cell **ngbs;

}Cell;

void init_board(Cell ** board);;
void set_board_neighbours(Cell **board);
void print_board(Cell **board);
void set_board(Cell** board, char* word);
void print_choice(Cell **board);
void shuffle_board(Cell **board);
int score(Cell **board, char *word);
void free_board(Cell **board);

#endif
