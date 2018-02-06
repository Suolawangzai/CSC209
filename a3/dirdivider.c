#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dirdivider.h"

/* Process all files in the directory  given and store them in the
 array provided and return the lengh of the file array. */
int create_file_array(char *directory, char *** file_array){
  DIR *d;
  struct dirent *dir;
  int file_counter = 0;
  int array_len = 0;

  // find the number of the files in the directory
  d = opendir (directory);
  //check error
  if (d == NULL) {
    fprintf(stderr, "Error opening input directory \n");
    exit(1);
  }
  while ((dir = readdir(d)) != NULL) {
    array_len++;
  }
  if (closedir (d)!= 0) {
    fprintf(stderr, "Error closing input directory\n");
    exit(1);
  }

  // open directory again to make a array of file names
  d = opendir (directory);
  //check error
  if (d == NULL) {
    fprintf(stderr, "Error opening directory \n");
    exit(1);
  }

       // assign memory to the file array
  *file_array = malloc(sizeof(char*)* array_len);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      // extract file name and store it in the file_array
      char *file_name = dir->d_name;
      // ignore the hidden files
      if(file_name[0] != '.'){
        (*file_array)[file_counter] = malloc(sizeof(char)*
                                     (strlen(directory) + strlen(file_name) + 2));
        strcpy((*file_array)[file_counter], directory);
        strcat((*file_array)[file_counter], "/\0");
        strcat((*file_array)[file_counter], file_name);
        (*file_array)[file_counter][strlen(directory) +
                                          strlen(file_name) + 1] = '\0';
        file_counter++;
      }
    }
  }
  if (closedir (d)!= 0) {
    fprintf(stderr, "Error when close input directory \n");
    exit(1);
  }
    return file_counter;
}

/* Clean the dynamiclly allocated string array *file_array */
void clean_file_array(char ***file_array, int len){
    int i;
    for(i = 0; i < len; i++){
      free((*file_array)[i]);
    }
      free(*file_array);
}
