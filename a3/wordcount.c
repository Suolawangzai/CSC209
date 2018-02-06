#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wordcount.h"

#define MAX_LINE_LEN 1024

/* Count the number occurence of the word of given length in the
 file of name file_name and update the occurence of the word of
 given length in word_len_array that has maximum word length of len. */
void wordcount(char *file_name, long *word_len_array, int len){
    FILE *inputfp;
    inputfp = fopen(file_name, "r");

    //check error
    if (inputfp == NULL) {
        fprintf(stderr, "Error opening input file \n");
        exit(1);
    }

    char *pch;
    char delimiter[] = " ,.-!?\"|':;+";
    char str[MAX_LINE_LEN];
    while((fgets(str, MAX_LINE_LEN * sizeof(char), inputfp))){
        str[strcspn(str, "\r\n")] = '\0';
        // parse the string str into words
        pch = strtok (str,delimiter);
        while (pch != NULL) {
            int word_len = strlen(pch);
            if(word_len > 0 && word_len < len){
                word_len_array[word_len]++;
            }
            pch = strtok (NULL,delimiter);
        }
    }
    //close the file and check error
    if (fclose(inputfp)!= 0) {
        fprintf(stderr, "fclose failed on input \n");
        exit(1);
    }
}

/* Add two int arrays together element-wise and store the result in array1.
 Two array provided must be of the same length. */
void array_add(long *array1, const long *array2, int len){
    int i;
    for(i = 0; i < len; i++){
        array1[i] += array2[i];
    }
}
