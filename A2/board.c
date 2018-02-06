#include "board.h"

char Die[16][6] = {	{'R', 'I','F','O','B','X'}, {'I', 'F','E','H','E','Y'},
					{'D', 'E','N','O','W','S'}, {'U', 'T','O','K','N','D'},
					{'H', 'M','S','R','A','O'}, {'L', 'U','P','E','T','S'},
					{'A', 'C','I','T','O','A'}, {'Y', 'L','G','K','U','E'},
					{'Q', 'B','M','J','O','A'}, {'E', 'H','I','S','P','N'},
					{'V', 'E','T','I','G','N'}, {'B', 'A','L','I','Y','T'},
					{'E', 'Z','A','V','N','D'}, {'R', 'A','L','E','S','C'},
					{'U', 'W','I','L','R','G'}, {'P', 'A','C','E','M','D'} };

/*
* set each cell to un-visited
* 0 un-visited, 1 vistied;
*/
void init_board(Cell **board){

	int i;
	for (i = 0; i < 16; i++){
		(*board)[i].visited = 0;
	}
}

/*
* set neighbours of each cell
*/
void set_board_neighbours(Cell **board){
	int i, j, k;
	for(i = 0; i < 16; i++){

		// init vars
		int num_ngb = 0, ngb[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

		// check top
		if(i - 4 >= 0){ num_ngb++; ngb[0] = i - 4; }
		// check r_top
		if((i - 4 >= 0) && (i % 4 != 3)){ num_ngb++; ngb[1] = i - 3;}
		// check right
		if(i % 4 != 3){	num_ngb++; ngb[2] = i + 1;}
		// check b_right
		if((i + 4 < 16) && (i % 4 != 3)){ num_ngb++; ngb[3] = i + 5;}
		// check bottom
		if(i + 4 < 16){num_ngb++; ngb[4] = i + 4;}
		// check l_bottom
		if((i + 4 < 16) && (i % 4)){ num_ngb++; ngb[5] = i + 3;}
		// check left
		if((i % 4)){ num_ngb++; ngb[6] = i - 1;}
		// check l_top
		if((i - 4 >= 0) && (i % 4)){ num_ngb++; ngb[7] = i - 5;}

		// init ngbs of board
		k = 0;
		(*board)[i].num_ngbs = num_ngb;
		(*board)[i].ngbs = malloc(num_ngb * sizeof(Cell *));
		for(j = 0; j < 8; j++){
			if(ngb[j] >= 0){
				(*board)[i].ngbs[k] = &(*board)[ngb[j]];
				k++;
			}
		}
	}
}

/*
* print the boggle game borad in gird form
*/
void print_board(Cell **board){
	int i,j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			printf("\t %c", (*board)[i * 4 + j].letter);
		}
		printf("\n");
	}
}

/*
* print the boggle game borad in gird form
*/
void print_choice(Cell **board){
	int i,j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			printf("\t %d", (*board)[i * 4 + j].visited);
		}
		printf("\n");
	}
}

/*
* suffle the boggle board, assign letter to
* every grid.
*/
void shuffle_board(Cell **board){
	int i, new_index, temp;
	int array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	// shuffle the array 100 times
	srand(time(NULL));
	for (i = 0; i < 16; i++) {
	  new_index = rand() % 16;
	  temp = array[i];
	  array[i] = array[new_index];
	  array[new_index] = temp;
	}
	srand(time(NULL));
	// assign letter to each cell
	for (i = 0; i < 16; i++){
		(*board)[i].letter = Die[array[i]][rand() % 6];
	}
}


/*
* check the 'Qu' condition i.e if 'Q' occurs then 'u' is right after it
*/
int check_Qu(char * word){
	int i;
	for (i = 0; i < strlen(word); i++){
		if(word[i] == 'Q'){
			if((i == (strlen(word)-1)) || !(word[i+1] == 'U') ){
				return 0;
			}
		}
	}
	return 1;
}

int findable_helper(Cell *cur, char* word){
	int i, j,len = strlen(word);
	if(!len){
		return 1;
	}
	else{
		for(i = 0; i < cur->num_ngbs; i++){

			if(!(cur->ngbs[i]->visited) && (cur->ngbs[i]->letter == *word)){
				cur->ngbs[i]->visited = 1;
				if(*word == 'Q'){
					if(findable_helper(cur->ngbs[i], word+2)){
						return 1;
					}
					cur->ngbs[i]->visited = 0;
				}
				else{
					if(findable_helper(cur->ngbs[i], word+1)){
						return 1;
					}
					cur->ngbs[i]->visited = 0;
				}
			}
		}
		return 0;
	}
}

/*
* check if the word is findable in borad
*/
int findable(Cell **board, char *word){
	int i,j;
	for (i = 0; i < 16; i++){
		if((*board)[i].letter == *word){
			(*board)[i].visited = 1;
			if(*word == 'Q'){
				if(findable_helper(&((*board)[i]), word+2)){
					return 1;
				}
				(*board)[i].visited = 0;
			}
			else{
				if(findable_helper(&((*board)[i]), word+1)){
					return 1;
				}
				(*board)[i].visited = 0;
			}
		}
	}
	return 0;
}

/*
* return the score based on scoring rules, first check if the word is
* findable in the board
*/
int score(Cell **board, char *word){
	int len = strlen(word);
	if(len > 2){
		if(check_Qu(word)){
			if(findable(board, word)){
				if(len <= 4) return 1;
				if(len == 5) return 2;
				if(len == 6) return 3;
				if(len == 7) return 5;
				if(len >= 8) return 11;
			}
		}
	}
	return 0;
}

/*
* free board
*/
void free_board(Cell** board){
	int i, j;
	for (i = 0; i < 16; i++){
		free((*board)[i].ngbs);
	}
	free(*board);
}

/*
* set board based on string
*/
void set_board(Cell** board, char* word){
	int i;
	set_board_neighbours(board);
	for(i = 0; i < 16; i++){
		(*board)[i].letter = word[i];
	}
}
