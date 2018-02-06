#ifndef DIRDIVIDER_H
#define DIRDIVIDER_H

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int create_file_array(char *directory, char ***file_array);
void clean_file_array(char ***file_array, int len);


#endif
