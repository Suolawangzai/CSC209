#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include "game.h"

#define MAX_CLIENTS  (FD_SETSIZE)
#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 12
#define DELIM " \n\r"

#ifndef PORT
  #define PORT <51352>
#endif

/* 
 * Print a formatted error message to stderr.
 */
void error(int clientfd, char *msg) {
    client_printf(clientfd, "Error: %s\n", msg);
}

/*
 * discount a client form server 
 */
void client_close(int i, Client **clients){
    
    int fd = (*clients)[i].fd;
    (*clients)[i].fd = -1;
    (*clients)[i].set= 0;

    if (close(fd)) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

/* 
 * Read and process commands
 * Return:  -1 for quit command
 *          0 otherwise
 */
int process_args(int i, Client **clients, int cmd_argc, char **cmd_argv, Player **player_list_ptr) {
    Player *player_list = *player_list_ptr;

    if (cmd_argc <= 0) {
        return 0;
    } 
    else if (strcmp(cmd_argv[0], "all_players") == 0 && cmd_argc == 1) {
        list_players((*clients)[i].fd, player_list);
    } 
    else if (strcmp(cmd_argv[0], "top_3") == 0 && cmd_argc == 1) {
        top_3((*clients)[i].fd, player_list);
    } 
    else if (strcmp(cmd_argv[0], "add_score") == 0 && cmd_argc >= 2) {
        if (add_score((*clients)[i].name, atoi(cmd_argv[1]), player_list) == 1) {
            error((*clients)[i].fd,"player not found");
        }            
    } 
    else if (strcmp(cmd_argv[0], "new_game") == 0 && cmd_argc == 1) {   
       print_board ((*clients)[i].fd);
    } 
    else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
        client_printf((*clients)[i].fd, "Bye, Bye!\n");
        fprintf(stderr, "Client #%d disconnected\n", i);
        return -1;
    } 
    else {
        error((*clients)[i].fd, "a Incorrect syntax");
    }
    return 0;
}


/*
 * Tokenize the string stored in cmd.
 * Return the number of tokens, and store the tokens in cmd_argv.
 */
int tokenize(char *cmd, char **cmd_argv) {
    int cmd_argc = 0;
    char *next_token = strtok(cmd, DELIM);    
    while (next_token != NULL) {
        if (cmd_argc >= INPUT_ARG_MAX_NUM - 1) {
            //error("Too many arguments!");
            cmd_argc = 0;
            break;
        }
        cmd_argv[cmd_argc] = next_token;
        cmd_argc++;
        next_token = strtok(NULL, DELIM);
    }

    return cmd_argc;
}

/**
 * Handle a pending client connection on `sockfd`, finding a suitable location
 * in `client_fds` to store the descriptor.
 */
static void handle_accept(int sockfd, Client **clients)
{
    int i;

    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof (client_addr);

    int fd = accept(sockfd, (struct sockaddr *) &client_addr, &addrlen);
    
    if (fd < 0) {
        perror("accept");
        return;
    }

    // Find an entry in the `client_fds` array
    for (i = 0; i < MAX_CLIENTS; i++) {
        if ((*clients)[i].fd== -1) {
            break;
        }
    }

    fprintf(stdout, "num: %d\n", i );
    if (i == MAX_CLIENTS) {
        fprintf(stderr, "Too many clients, closing new connection...\n");
        close(fd);
    }

    (*clients)[i].fd = fd;
    (*clients)[i].set = 0;

    client_printf((*clients)[i].fd, "Please enter your name:\n");

    fprintf(stderr, "Client #%d connected from %s:%d\n",
            i,
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));
}

/**
 * Handle activity on the client socket descriptor
 */

static int handle_client_readiness(int num, Client **clients, Player **players)
{
    char buf[512 + 1];
    ssize_t bytes_read = read((*clients)[num].fd, buf, sizeof (buf) - 1);

    if (bytes_read < 0) {
        perror("read");
        return -1;
    }
    else if (bytes_read == 0) {
        // A `read` return value of 0 indicates end-of-stream
        fprintf(stderr, "Client #%d disconnected\n", num);
        return -1;
    }
    else {
        // Ensure buf is a C string (we were careful about the sizing above)
        buf[bytes_read] = '\0';

        //Strip out last newline for printing convenience
        
        char *nl = strrchr(buf, '\n');
        
            if (nl != NULL) {
                *nl = '\0';
        }
        char *rl = strrchr(buf, '\r');
        
            if (rl != NULL) {
                *rl = '\0';
        }
        
        if ((*clients)[num].set){
            char *cmd_argv[INPUT_ARG_MAX_NUM];
            int cmd_argc = tokenize(buf, cmd_argv);
            
            fprintf(stdout, "%s said: %s\n", (*clients)[num].name, cmd_argv[0]);
            if(process_args(num, clients, cmd_argc, cmd_argv, players) == -1){
                return -1;
            }
            
        }
        if(!(*clients)[num].set){

            switch (add_player(buf, players)) {
                case 0:
                    strncpy((*clients)[num].name, buf,  MAX_NAME);
                    (*clients)[num].set = 1;
                    client_printf((*clients)[num].fd, "%s welcome to play the game! \n", (*clients)[num].name);
                    break;
                case 1:
                    error((*clients)[num].fd, "player by this name already exists");
                    client_printf((*clients)[num].fd, "Please enter different name:\n");
                    break;
                case 2:
                    error((*clients)[num].fd, "playername is too long");
                    break;
            }           
        }    
    }
    return (*clients)[num].fd;
}

int main(int argc, char *argv[])
{
    int i;
    Player *player_list = NULL;

    char *hostname = "wolf.cdf.toronto.edu";
    int port = PORT;

    // Create socket
    // AF_INET for IPv4 Internet protocols
    // Provides sequenced, reliable, two-way, connection-based byte streams.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Enable the SO_REUSEADDR option on sockfd
    // SO_REUSEADDR: allow reuse of local addresses
    int optval = 1; // Boolean true

    if (setsockopt(
                sockfd,
                SOL_SOCKET,
                SO_REUSEADDR,
                (void *) &optval,
                sizeof (optval)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind to host & port
    struct sockaddr_in addr;
   
    struct hostent *hp = gethostbyname(hostname);
    if (hp == NULL) {
        perror("gethostbyname");
        return 0;
    }

    struct in_addr *in4 = (struct in_addr *) hp->h_addr_list[0];
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *in4;

    if (bind(sockfd, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Prepare for new connections
    if (listen(sockfd, 128) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Listening for connections on %s:%d\n",
            inet_ntoa(addr.sin_addr),
            ntohs(addr.sin_port));

    // Initialize client FD array to all -1 (indicating unused entry)
    Client *clients;
    clients = (Client *) malloc(MAX_CLIENTS * sizeof(Client));
    
    for (i = 0; i < MAX_CLIENTS; i++) {
        clients[i].fd = -1;
    }

    while (1) {
        // Setup the read set with the listening FD and any connected clients
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        int max_fd = sockfd;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].fd == -1) {
                continue;
            }

            // Add client socket FD to set
            FD_SET(clients[i].fd, &readfds);

            if (clients[i].fd > max_fd) {
                max_fd = clients[i].fd;
            }
        }

        
        // Call select
        int rc = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (rc < 0) {
            switch (errno) {
            case EINTR:
                fprintf(stderr, "select() returned EINTR, continuing\n");
                continue;

            default:
                perror("select");
                exit(EXIT_FAILURE);
            }
        }

        // Are there new connections on the listening socket?
        if (FD_ISSET(sockfd, &readfds)) {
            handle_accept(sockfd, &clients);
        }


        for (i = 0; i < MAX_CLIENTS; i++) {
            // No client FD in this entry
            if (clients[i].fd == -1) {
                continue;
            }

            // select did not indicate readiness fro this FD
            if (!FD_ISSET(clients[i].fd, &readfds)) {
                continue;
            }

            // Handle client FD
            int fd = handle_client_readiness(i, &clients, &player_list);
            if( fd == -1){
                client_close(i, &clients);
            }
            clients[i].fd = fd;
        }
    }

    close(sockfd);

    return EXIT_SUCCESS;
}

