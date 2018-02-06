#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void wordcount(char *file_name, long *word_len_array, int len);
void array_add(long *array1, const long *array2, int len);

#endif
