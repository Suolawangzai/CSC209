#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Read a user id and password from standard input,
   - create a new process to run the validate program
   - send 'validate' the user id and password on a pipe,
   - print a message
        "Password verified" if the user id and password matched,
        "Invalid password", or
        "No such user"
     depending on the return value of 'validate'.
*/


int main(void) {
    char userid[10];
    char password[10];
    /* new variable */
    int fd[2], r, status;

    /* Read a user id and password from stdin */
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);

    /*Your code here*/
    if(pipe(fd)== -1){
      perror("pipe");
      exit(1);
    }

    if((r = fork()) > 0){  //parent process
      // close read file descriptor since parent will write to pipe
      if(close(fd[0]) == -1){
        perror("close");
        exit(1);
      }
      // parent writes to pipe
      if(write(fd[1], userid, 10) == -1){
        perror("write to pipe");
        exit(1);
      }
      if(write(fd[1], password, 10)== -1){
        perror("write to pipe");
        exit(1);
      }
      // close the file desciptor
      if(close(fd[1]) == -1){
        perror("close");
        exit(1);
      }

      // wait for child process and print out the result
      if(wait(&status) == -1){
        perror("wait");
        exit(1);
      }
      else{
        if(WIFEXITED(status)){
          int child = WEXITSTATUS(status);
          if(child == 0){
            printf("Password verified. \n");
          }
          else if(child == 2){
            printf("Invalid password. \n");
          }
          else if(child == 3){
            printf("No such user. \n");
          }
          else{ // error occured
            exit(1);
            printf("Program exit abnomaly \n");
          }
        }
      }
    }

    else if(r == 0){  // child process
      // close write file descriptor of child process
      if(close(fd[1]) == -1){
        perror("close");
        exit(1);
      }
      // reset stdin of child process
      if(dup2(fd[0], fileno(stdin)) == -1){
        perror("dup2");
        exit(1);
      }

      // close file descriptor
      if(close(fd[0]) == -1){
        perror("close");
        exit(1);
      }

      execl("./validate", "validate", (char *) 0);
      perror("exec");
      return 1;
    }

    else{
      perror("fork");
      exit(1);
    }

    return 0;
}
