#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

int num_reads = 0;
int s = 1;
void new_handler(int sig);

int main(int argc, char **argv){
  FILE *fp;
  int i, j;
  if(argc != 2){
    fprintf(stderr, "Need to provide argument of file name \n");
    exit(1);
  }


  // Set signal handler
  struct sigaction newact;
  newact.sa_handler = new_handler;
  newact.sa_flags = 0;
  sigemptyset(&newact.sa_mask);
  sigaction(SIGALRM, &newact, NULL);

  char* file_name = argv[1];
  fp = fopen(file_name, "rb");
  if(fp == NULL){
    fprintf(stderr, "Error in opening file \n");
    exit(1);
  }

  // Set up the timer
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_value.tv_sec = s;
  timer.it_interval.tv_usec = 0;
  timer.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &timer, NULL);

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGALRM);
  while(fread(&i, 1, sizeof(int), fp) > 0){
      j = random() % 100;
      fseek(fp, j * sizeof(int), SEEK_SET);
      sigprocmask(SIG_BLOCK, &set, NULL);
      printf("Reading offset: %lu\n", j * sizeof(int));
      sigprocmask(SIG_UNBLOCK, &set, NULL);
      num_reads++;
  }

  if(fclose(fp) == -1){
    fprintf(stderr, "Error in closing file \n");
    exit(1);
  }

  //printf("Program about to end \n");
  return 0;
}

void new_handler(int sig){
  fprintf(stdout, "Alarm received \n");
  fprintf(stdout, "The number of second is %d and number of "
                    "reads complete is %d\n", s, num_reads);
  exit(0);
}
