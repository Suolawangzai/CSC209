#ifndef GAME_H
#define GAME_H

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#define MAX_NAME 10     // Max playername 

//Node for linked list which stores all players
typedef struct player {
    char name[MAX_NAME];  
    int max_score;
    int total_games;
    int total_score; 
    struct player *next;
} Player;

typedef struct client {
    char name[MAX_NAME];  
    int fd;
    int set;
} Client;

int add_player(const char *name, Player **user_ptr_add);

Player *find_player(const char *name, const Player *head);

void list_players(int clientfd, const Player *curr);

void print_board(int clientfd);

int print_player(int clientfd, Player *player);

int delete_player(char *name);

int add_score(char *name, int score, const Player *head);

int client_printf(int clientfd, const char *format, ...);

int client_write(int clientfd, const void *buf, size_t nbyte);

void top_3(int clientfd, Player *curr);

#endif
