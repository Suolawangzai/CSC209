#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "user.h"
#include "board.h"
#include "dictionary.h"

#define DIC_NAME "EnglishWords.txt"
#define MAX_LINE 100

/*
* convert the word in upper case
*/
void upper(char * word){
	int i = 0;
	while (word[i]){
		word[i] = toupper(word[i]);
		i++;
	}
}

/*
* convert the word in lower case
*/
void lower(char * word){
	int i = 0;
	while (word[i]){
		word[i] = tolower(word[i]);
		i++;
	}
}

/*
* convert the first letter cap the rest lower
*/
void first_cap(char * word){
	int i = 0;
	while (word[i]){
		word[i] = tolower(word[i]);
		if(!i){
			word[i] = toupper(word[i]);
		}
		i++;
	}
}

/*
* check if the given word is dic
*/
int get_point(DNode* dictionary[], DNode* user_input[],	Cell** board, char * word){
	int point;
	upper(word);
	point = score(board, word);
	// already in user input
	if(lookup (user_input, SMALL_HASH_SIZE, word) != NULL){
		return 0;
	}
	// not in dic
	lower(word);
	if(lookup(dictionary, BIG_HASH_SIZE, word) == NULL){
		first_cap(word);
		if(lookup(dictionary, BIG_HASH_SIZE, word) == NULL){
			upper(word);
			return 0;
		}
	}
	upper(word);
	return point;
}

/*
* read a test file run the test case of a game, the
* test file has two lines
*/
int test(char *filename){
	int i, point, total_point = 0;
	Cell *board;
	FILE *test_FP, *input_FP, *result_FP;
	char line [MAX_LINE];
	char firstline [MAX_LINE];
	char secondline [MAX_LINE];
	static DNode* dictionary [BIG_HASH_SIZE];
	static DNode* user_input [SMALL_HASH_SIZE];
	//init english dic
	if(!(input_FP= fopen ( DIC_NAME , "r" ))) {
    	fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n", DIC_NAME );
    	return 1;
	}

	while( fgets (line, MAX_LINE, input_FP)!=NULL ) {
		line[strcspn(line, "\r\n")] = '\0';  //trim new line characters
		insert (dictionary, BIG_HASH_SIZE, line);
	}
	fclose (input_FP);

	// read test file
	if(!(test_FP= fopen (filename, "r" ))) {
			fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n", filename);
			return 1;
	}
	fgets (firstline, MAX_LINE, test_FP);
	fgets (secondline, MAX_LINE, test_FP);
	fclose (test_FP);
	// init board
	board = (Cell *) malloc(16 * sizeof(Cell));
	set_board(&board, firstline);

	// check words
	if(!(result_FP= fopen ("result.txt", "w" ))){
			freopen("result.txt", "w", result_FP);
	}
	char * word;
	char temp[128];
	int count = 0;
 	word = strtok (secondline,", '\n'");
	strncpy(temp, word, 128);
 	while (word != NULL){
		init_board(&board);
		point = get_point(dictionary, user_input,	&board, word);
		if(point){
			total_point += point;
			insert (user_input, SMALL_HASH_SIZE, word);
		}
		else{
			if(!count){
					fprintf(result_FP, "%s", temp);
			}
			else{
				  fprintf(result_FP, ",%s", temp);
			}
			count++;
		}
		word = strtok (NULL, ",");
		if(word != NULL){
				 strncpy(temp, word, 128);
		}
 	}
	fprintf(result_FP, "\n");
	fprintf(result_FP, "%d", total_point);

	free_board(&board);
	free_dictionary(user_input, SMALL_HASH_SIZE);
	free_dictionary(dictionary, BIG_HASH_SIZE);
	
	return 0;
}

/*
* run the normal game
*/
int game(){
	int point;
	Cell *board;
	User *lst = NULL;
	FILE *input_FP;
	char input[32] = "1";
	char line [MAX_LINE];
	static DNode* dictionary [BIG_HASH_SIZE];
	// init board info
	board = (Cell *) malloc(16 * sizeof(Cell));
	set_board_neighbours(&board);
	shuffle_board(&board);
	init_board(&board);
	// init user info
	init_user_info(&lst);
	// init english dic
	if(!(input_FP= fopen ( DIC_NAME , "r" ))) {
    	fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n",  DIC_NAME);
    	return 1;
	}
	while( fgets (line, MAX_LINE, input_FP)!=NULL ) {
		line[strcspn(line, "\r\n")] = '\0';  //trim new line characters
		insert (dictionary, BIG_HASH_SIZE, line);
	}
	fclose (input_FP);

	// game start
	while (1) {
		system(" clear ");
		//system(" clear && printf '\e[3J' ");
		int total_point = 0;
		static DNode* user_input [SMALL_HASH_SIZE];
		// dispaly score board when game start
		print_user_info(&lst);
		shuffle_board(&board);
		// generate game board
		print_board(&board);

		printf("Enter a word:\n");
		scanf("%s",input);

		while(strcmp(input, "n")){

			// game end
			if(!strcmp(input, "q")){
				system(" clear ");
				print_user_info(&lst);
				printf("game end !\n");
				free_user(&lst);
				free_board(&board);
				free_dictionary(user_input, SMALL_HASH_SIZE);
				free_dictionary(dictionary, BIG_HASH_SIZE);
				return 1;
			}
			// check word
			system("clear");
			char * word = input;
			point = get_point(dictionary, user_input,	&board, word);
			if(point){
				total_point += point;
				insert (user_input, SMALL_HASH_SIZE, word);
				printf("score %d point(s)\n", point);
			}
			else{
				printf("invaild word: %s\n", word);
			}

			printf("total points: %d\n", total_point);

			init_board(&board);
			print_board(&board);
			printf("next word:\n");
      scanf("%s",input);
		}
		free_dictionary(user_input,SMALL_HASH_SIZE);
		printf("Please enter your name: \n");
		scanf("%s",input);
		char * name = input;
		add_user(&lst, name, total_point, 1, total_point);
		save_user_info(&lst);
    }
	return 0;
}

int main(int argc, char ** argv) {

		if (argc >= 2) {
			//  test case
			char *  filename = argv [1];
			return test(filename);
		}
		else{
			// run game
			return game();
		}
}
