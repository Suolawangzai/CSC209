#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include "dirdivider.h"
#include "wordcount.h"

#define MAX_WORD_LEN 46

int main(int argc, char *argv[]) {
  int ch, i, num_proc = 1;
  char *infile_dir = NULL;
  char *outfile = NULL;
  // variables for timing
  struct timeval starttime, endtime;
  double timediff;

  // code for timimg
  if ((gettimeofday(&starttime, NULL)) == -1) {
    perror("gettimeofday");
    exit(1);
  }

  /* read in arguments */
  while ((ch = getopt(argc, argv, "d:n:o:")) != -1) {
      switch(ch) {
            case 'd': infile_dir = optarg;
                break;
            case 'n': num_proc = atoi(optarg);
                break;
            case 'o': outfile = optarg;
                break;
            default : fprintf(stderr, "Usage:  test -d <input file name> "
                              "-o <output file name>\n");
                exit(1);
        }
  }
  // check command-line arguments error
  if(infile_dir == NULL){
    fprintf(stderr, "Usage:  test -d <input file name> -o <output file name>\n");
    fprintf(stderr, "Need to provide input directory !\n");
    exit(1);
  }
  if(num_proc < 0){
    fprintf(stderr, "Usage:  test -d <input file name> -o <output file name>\n");
    fprintf(stderr, "Number of process must be non-negative!\n");
    exit(1);
  }

  if(num_proc == 0){
    num_proc = 1;
  }

  // Set up array of files
  char **file_array;
  int num_file = create_file_array(infile_dir, &file_array);

  // Set up word counter array
  long word_counts[MAX_WORD_LEN] = {0};
  long temp_word_counts[MAX_WORD_LEN] = {0};

  /* Need to consider when process number if greater than then
   the files numbers (undone)*/
  if(num_proc > num_file){
    num_proc = num_file;
  }

  // open the pipe
  int r, fd[2];
  if(pipe(fd)== -1){
    perror("pipe");
    exit(1);
  }

  // the files needed to be checked by parent process
  int end;
  if(num_proc == 1){
    end = num_file;
  }
  else{
    end = (int) (num_file/num_proc);
  }

  for(i = 0; i < end; i++){
    wordcount(file_array[i], word_counts, MAX_WORD_LEN);
  }

  // creating child processes
  for(i = 0; i < (num_proc - 1); i++){
    r = fork();
    if(r < 0){
      perror("fork");
      exit(1);
    }
    if(r == 0){  // child process
      // close read file descriptor of child process
      if(close(fd[0]) == -1){
        perror("close");
        exit(1);
      }

      // allocate files in th file array for process to work on
      int start_index = ((int) (num_file/num_proc)) * (i+1);
      int end_index;
      // last the process take all the files left
      if(i == (num_proc - 2)){
        end_index = num_file;
      }
      else{
        end_index = ((int) (num_file/num_proc)) * (i+2);
      }

      for(i = start_index; i < end_index; i++){
        long file_word_count[MAX_WORD_LEN] = {0};
        wordcount(file_array[i], file_word_count, MAX_WORD_LEN);
        if(write(fd[1], file_word_count, MAX_WORD_LEN * sizeof(long)) == -1){
          perror("write to pipe");
          exit(1);
        }
      }
      // close writing channel
      if(close(fd[1]) == -1){
        perror("close");
        exit(1);
      }
      // clean up file array for children
      clean_file_array(&file_array, num_file);
      exit(0);
    }
  }

  // close parent write end of pipe
  if(close(fd[1]) == -1){
    perror("close");
    exit(1);
  }

  // read result from child processes
  int count = 0;
  if(num_proc > 1){
    int error;
    while((error = read(fd[0], temp_word_counts, MAX_WORD_LEN * sizeof(long))) > 0){
      array_add(word_counts, temp_word_counts, MAX_WORD_LEN);
      count++;
    }
    if(error == -1){
      perror("read from pipe");
      exit(1);
    }
  }

  // close read file descriptor of pipe of parent process
  if(close(fd[0]) == -1){
    perror("close");
    exit(1);
  }
  // writing  ouput file to specified file
  if(outfile){
    FILE *outputfp = fopen(outfile, "wb");
    if(outputfp == NULL){
      fprintf(stderr, "Error opening output file \n");
      exit(1);
    }
    for(i = 1; i < MAX_WORD_LEN; i++){
      fprintf(outputfp, "%d,%ld\n", i, word_counts[i]);
    }
    if (fclose(outputfp)!= 0) {
      fprintf(stderr, "fclose failed on output file\n");
      exit(1);
    }
  }
  else{
    for(i = 1; i < MAX_WORD_LEN; i++){
      printf("%d,%ld\n", i, word_counts[i]);
    }
  }
  // free the memory used
  clean_file_array(&file_array, num_file);

  // code for timimng
  if ((gettimeofday(&endtime, NULL)) == -1) {
    perror("gettimeofday");
    exit(1);
  }
  timediff = (endtime.tv_sec - starttime.tv_sec) +
    (endtime.tv_usec - starttime.tv_usec) / 1000000.0;
  fprintf(stderr, "%.4f\n", timediff);

  return 0;
}
