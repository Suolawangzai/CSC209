#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"


/*
* find user according to name in the link list.
* return a ptr of the desired user if found, otherwise return NULL
*/
User * find_user(User **lst, char *name){
	User *cur = *lst;
	while(cur != NULL){
		if(strcmp(cur->name, name) == 0){
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

/*
* add_user uses find_user to detemine whether user is already in the link_list
* if not, a new entry is created.
*/
void add_user(User **lst, char* name, int max_score, int total_games,
	int total_scores){
	User *user, *cur;
	// init lst
	if((*lst) == NULL){
		// malloc space for lst
		(*lst) = (User*) malloc(sizeof(User));
		// set paras
		(*lst)->name = malloc(sizeof(char)* (strlen(name) + 1));
		strcpy((*lst)->name, name);
		if((*lst)->max_score < max_score){
			(*lst)->max_score = max_score;
		}
		(*lst)->total_games = total_games;
		(*lst)->total_scores = total_scores;
		(*lst)->next = NULL;
	}
	else{
		// check if the user already exist
		if((user = find_user(lst, name)) == NULL){
			cur = *lst;
			while(cur->next != NULL){
				cur = cur->next;
			}
			// malloc space for new user
			cur->next = (User*) malloc(sizeof(User));
			// set paras
			cur->next->name = malloc(sizeof(char)* (strlen(name) + 1));
			strcpy(cur->next->name, name);
			cur->next->max_score = max_score;
			cur->next->total_games = total_games;
			cur->next->total_scores = total_scores;
			cur->next->next = NULL;
		}
		else{
			if(user->max_score < max_score){
				user->max_score = max_score;
			}
			user->total_games += total_games;
			user->total_scores += total_scores;
		}
	}
}

/*
* read score board info from score board file
*/
void init_user_info(User ** lst){
	FILE *score_FP;
	char name[NAME_LEN];
	int max_score, total_games, total_scores;

	if(!(score_FP= fopen ( SCORE_BOARD_FILE , "r" ))){
		fprintf(stderr,"Could not open file: %s ", SCORE_BOARD_FILE);
	}
	else{
		while(fscanf(score_FP, "%128s %d %d %d", name, &max_score,
			&total_games, &total_scores) == 4){
			add_user(lst, name, max_score, total_games, total_scores);
		}
		fclose(score_FP);
	}

}

/*
* print the user info
*/
void print_user_info(User **lst){
	if((*lst) == NULL){
		fprintf(stdout, "No user info yet!\n");
	}
	else{
		User *cur = *lst;
		fprintf(stdout, "%s %13s %13s %15s\n",
			"Username", "Max_score", "Total_game", "Total_score");
		while(cur != NULL){
			fprintf(stdout, "%s\t\t%d \t\t%d \t\t%d\n", cur->name, cur->max_score,
				cur->total_games, cur->total_scores);
			cur = cur->next;
		}
	}
	printf("\n");
}

/*
* save the score board info into file
*/
void save_user_info(User **lst){
	User * cur = *lst;

	FILE *score_FP = fopen(SCORE_BOARD_FILE , "w");

	while(cur != NULL){
		fprintf(score_FP, "%s \t %d \t %d \t %d \n", cur->name, cur->max_score,
			cur->total_games, cur->total_scores);
		cur = cur->next;
	}
	fclose(score_FP);
}

/*
* free lst user
*/
void free_user(User ** lst){
   User* tmp;
   while ((*lst) != NULL){
       tmp = (*lst);
       (*lst) = (*lst)->next;
       free(tmp->name);
       free(tmp);
    }
}
