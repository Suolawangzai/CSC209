#include <stdio.h>
#include <string.h>
#include "dictionary.h"
#define MAX_LINE 100

int main (int argc, char ** argv) {
	int i;
	FILE *input_FP;
	char line [MAX_LINE];
	char * file_name;
	DNode* result;
	static DNode* big_dictionary [BIG_HASH_SIZE];
	static DNode* small_dictionary [SMALL_HASH_SIZE];
	

	if (argc < 2) {
		fprintf (stderr, "test_dictionary <dictionary file name>\n");
		return 1;
	}
	file_name = argv [1];

	if(!(input_FP = fopen ( file_name , "r" )))    {
        fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n", file_name);
        return 1;
    }

	while( fgets (line, MAX_LINE, input_FP)!=NULL ) {
		line[strcspn(line, "\r\n")] = '\0';  //trim new line characters
		insert (big_dictionary, BIG_HASH_SIZE, line);
	}	
	fclose (input_FP);

	result = lookup (big_dictionary, BIG_HASH_SIZE, "ape");
	if (result != NULL) 
		printf ("<ape> is in the dictionary\n");
	else
		printf ("<ape> not found\n");
	
	result = lookup (big_dictionary, BIG_HASH_SIZE, "Macac");
	if (result != NULL) 
		printf ("<macac> is in the dictionary\n");
	else
		printf ("<macac> not found\n");

	free_dictionary(big_dictionary, BIG_HASH_SIZE);

	printf ("\n new game session: 1\n");
	//now testing small dictionary - twice - with cleanup
	for (i =0; i< 5; i++) {
		result = lookup (small_dictionary, SMALL_HASH_SIZE, "arc");
		if (result == NULL) {
			insert (small_dictionary, SMALL_HASH_SIZE, "arc");
			printf ("Successfully inserted <arc> in session 1\n");
		}
		else
			printf ("<arc> has been already used in session 1\n");
	}
		
	free_dictionary(small_dictionary, SMALL_HASH_SIZE);

	printf ("\n new game session: 2\n");
	for (i =0; i< 5; i++) {
		result = lookup (small_dictionary, SMALL_HASH_SIZE, "arc");
		if (result == NULL){
			insert (small_dictionary, SMALL_HASH_SIZE, "arc");
			printf ("Successfully inserted <arc> in session 2\n");
		}
		else
			printf ("<arc> has been already used in session 2\n");
	}
		
	free_dictionary(small_dictionary, SMALL_HASH_SIZE);

	return 0;
}