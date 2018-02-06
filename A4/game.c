#include "game.h"



/*
 * Create a new player with the given name.  Insert it at the tail of the list 
 * of players whose head is pointed to by *player_ptr_add.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a player by this name already exists in this list.
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator).
 */
int add_player(const char *name, Player **player_ptr_add) {
    if (strlen(name) >= MAX_NAME) {
        return 2;
    }

    Player *new_player = malloc(sizeof(Player));
    if (new_player == NULL) {
        perror("malloc");
        exit(1);
    }
    strncpy(new_player->name, name, MAX_NAME); // name has max length MAX_NAME - 1
    new_player->total_games =0;
    new_player->total_score =0;
    new_player->max_score =0;
    new_player->next = NULL;    

    // Add player to the end of the list
    Player *prev = NULL;
    Player *curr = *player_ptr_add;
    while (curr != NULL) {
        if(strcmp(curr->name, name) == 0){
            free(new_player);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        *player_ptr_add = new_player;
        return 0;
    } else {
        prev->next = new_player;
        return 0;
    }
}


/* 
 * Return a pointer to the player with this name in
 * the list starting with head. Return NULL if no such player exists.
 *
 * NOTE: You'll likely need to cast a (const Player *) to a (Player *)
 * to satisfy the prototype without warnings.
 */
Player *find_player(const char *name, const Player *head) {
/*    const Player *curr = head;
    while (curr != NULL && strcmp(name, curr->name) != 0) {
        curr = curr->next;
    }

    return (Player *)curr;
*/
    while (head != NULL && strcmp(name, head->name) != 0) {
        head = head->next;
    }

    return (Player *)head;
}


/*
 * Print the playernames of all players in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
void list_players(int clientfd, const Player *curr) {
    client_printf(clientfd,"Player List\n");
    while (curr != NULL) {
        client_printf(clientfd,"\t%s\ttotal games:%d\ttotal points:%d\tbest score:%d\n",
            curr->name, curr->total_games, curr->total_score, curr->max_score);
        curr = curr->next;
    }
}



/* 
 * Print player stats * 
 *   - 0 on success.
 *   - 1 if the player is NULL.
 */
int print_player (int clientfd, Player *p) {
    if (p == NULL) {
        return 1;
    }

    // Print name
    client_printf(clientfd,"Name: %s\n\n", p->name);
    client_printf(clientfd,"------------------------------------------\n");

    // Print player stats.
    client_printf(clientfd,"total games:%d, total points:%d best score:%d\n", p->total_games, p->total_score, p->max_score );
   
    client_printf(clientfd,"------------------------------------------\n");
   

    return 0;
}

/*
 * Finds the player and updates player score
 *
 * Return:
 *   - 0 on success
 *   - 1 if player is not in the list
 */
int add_score(char *name, int score, const Player *player_list){
    Player *player = find_player (name, player_list );
    if (player == NULL){
        return 1;
    } 
    player->total_games ++;
    player->total_score += score;
    if (player->max_score < score)
        player->max_score = score;
    return 0;
}


/*
 *generates a new game board abd prints it
 */
void print_board (int clientfd){
    //static gameboard to be returned - to be replaced by randomly generated boards
    int i=0, j=0;
    char board[4][4] = {{'a','c','s','m'},{'b','r','a','n'},{'d','y','s','e'},{'n','t','r','a'}};
    for (i=0; i<4; i++){
        for ( j=0; j<4; j++)
            client_printf(clientfd, "%c ",board[i][j]);
        client_printf(clientfd,"\n");
    }
}

int client_printf(int clientfd, const char *format, ...)
{

#   define BUF_SIZE     4096
    static char buf[BUF_SIZE];

    va_list vl;
    va_start(vl, format);

    int buf_len = vsnprintf(buf, sizeof (buf), format, vl);

    return client_write(clientfd, buf, buf_len);
}

int client_write(int clientfd, const void *buf, size_t nbyte)
{

    const void *cur = buf;
    const void *end = buf + nbyte;
    size_t bytes_left = nbyte;

    while (bytes_left > 0 && cur < end) {
        ssize_t written = write(clientfd, cur, bytes_left);

        switch (written) {
        case -1:
            // Error on this socket
            perror("write");
            return 0;

        case 0:
            return 0;

        default:
            cur += written;
            bytes_left -= written;
        }
    }

    return 1;
}


void top_3(int clientfd, Player *curr){
    char *n0, *n1, *n2, *ns0, *ns1, *ns2;
    int  ts0 = -1, ts1 = -1, ts2 = -1, t0 = -1, t1= -1, t2 = -1;

    while (curr != NULL) {

        if(t0 >= t2 && t2 >= t1){
            ts0 = t0;
            ts1 = t2;
            ts2 = t1;
            ns0 = n0;
            ns1 = n2;
            ns2 = n1;
        }
        else if(t1 >= t0 && t0>= t2){
            ts0 = t1;
            ts1 = t0;
            ts2 = t2;
            ns0 = n1;
            ns1 = n0;
            ns2 = n2;
        }
        else if(t1 >= t2 && t2>= t0){
            ts0 = t1;
            ts1 = t2;
            ts2 = t0;
            ns0 = n1;
            ns1 = n2;
            ns2 = n0;
        }
        else if(t2 >= t0 && t0>= t1){
            ts0 = t2;
            ts1 = t0;
            ts2 = t1;
            ns0 = n2;
            ns1 = n0;
            ns2 = n1;
        }
        else if(t2 >= t1 && t1 >= t0){
            ts0 = t2;
            ts1 = t1;
            ts2 = t0;
            ns0 = n2;
            ns1 = n1;
            ns2 = n0;
        }
        else{
            ts0 = t0;
            ts1 = t1;
            ts2 = t2;
            ns0 = n0;
            ns1 = n1;
            ns2 = n2;
        }
        
        t0 = ts0;
        t1 = ts1;
        t2 = ts2;
        n0 = ns0;
        n1 = ns1;
        n2 = ns2;

        if(t0 == -1){
            t0 = curr->max_score;
            n0 = curr->name;
        }
        else if(t1 == -1){
            t1 = curr->max_score;
            n1 = curr->name;
        }
        else if(t2 == -1){
            t2 = curr->max_score;
            n2 = curr->name;
        }
        else if(curr->max_score > t2){
            t2 = curr->max_score;
            n2 = curr->name;   
        }

        curr = curr->next;
    }

        if(t0 >= t2 && t2 >= t1){
            ts0 = t0;
            ts1 = t2;
            ts2 = t1;
            ns0 = n0;
            ns1 = n2;
            ns2 = n1;
        }
        else if(t1 >= t0 && t0>= t2){
            ts0 = t1;
            ts1 = t0;
            ts2 = t2;
            ns0 = n1;
            ns1 = n0;
            ns2 = n2;
        }
        else if(t1 >= t2 && t2>= t0){
            ts0 = t1;
            ts1 = t2;
            ts2 = t0;
            ns0 = n1;
            ns1 = n2;
            ns2 = n0;
        }
        else if(t2 >= t0 && t0>= t1){
            ts0 = t2;
            ts1 = t0;
            ts2 = t1;
            ns0 = n2;
            ns1 = n0;
            ns2 = n1;
        }
        else if(t2 >= t1 && t1 >= t0){
            ts0 = t2;
            ts1 = t1;
            ts2 = t0;
            ns0 = n2;
            ns1 = n1;
            ns2 = n0;
        }
        else{
            ts0 = t0;
            ts1 = t1;
            ts2 = t2;
            ns0 = n0;
            ns1 = n1;
            ns2 = n2;
        }
        
        t0 = ts0;
        t1 = ts1;
        t2 = ts2;
        n0 = ns0;
        n1 = ns1;
        n2 = ns2;

    if(t0 != -1){
       client_printf(clientfd,"TOP1: %s\t%d\n", n0, t0); 
    }
    if(t1 != -1){
       client_printf(clientfd,"TOP2: %s\t%d\n", n1, t1); 
    } 
    if(t2 != -1){
       client_printf(clientfd,"TOP3: %s\t%d\n", n2, t2); 
    }  
}