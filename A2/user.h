#ifndef USER_H
#define USER_H

#include <stdio.h>

#define SCORE_BOARD_FILE "score_board.txt"
#define NAME_LEN 128

typedef struct user{
	char *name;
	int max_score;
	int total_games;
	int total_scores;
	struct user *next;
}User;

void add_user(User **lst, char* name, int max_score, int total_games,
	int total_scores);
void init_user_info(User **  lst);
void print_user_info(User ** lst);
void save_user_info(User ** lst);
void free_user(User ** lst);

#endif
