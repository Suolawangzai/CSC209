#include <stdio.h>
#include <string.h>
#include "board.h"

void test_set_neighbours(Cell ** board){
	
	set_neighbours(board);

	int i, j;
	for (i = 0; i < 16; i++){
		printf("%d:", i);
		for(j = 0; j < (*board)[i].num_ngbs; j++){
			printf("\t %d", (*board)[i].ngbs[j]->visited);
		}
		printf("\n");
	}
}

void test_generate_board(Cell ** board){

	generate_board(board);
	
}

void test_checker(Cell ** board){
	init(board);
	set_neighbours(board);
	generate_board(board);
}

int main(){

	int i;
	char *word;
	Cell *board;
	board = (Cell *) malloc(16 * sizeof(Cell));
	init(&board);
	char input[128];

	test_checker(&board);
	scanf("%s", input);
	printf("\n");
	word = input;
	if(score(&board, word)){
		printf("yes\n");
	}


	print_choice(&board);
	return 1;
}
