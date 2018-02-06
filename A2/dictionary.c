
#include <string.h>
#include "dictionary.h"


//form hash value for string s
//this produces a starting value in the dictionary array
unsigned hash(const char *s) {
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval ;
}

//Performs search for a key in the hashtable.
//if the string s is in the dictionary, it is in the list of blocks 
//beginning in array entry hash(s).
//if lookup finds entry for s, it returns a pointer to the node
//if not - it returns NULL
DNode * lookup (DNode ** dictionary, int hash_size, const char *key) {
	DNode * np;
	unsigned int hashval = hash(key);
	for (np = dictionary [hashval % hash_size]; np !=NULL; np = np->next)
		if (strcmp (key, np->key) == 0)
			return np; //found
	return NULL; //not found
}

//insert uses lookup to detemine whether key is already in the dictionary
//if not, a new entry is created
DNode * insert (DNode ** dictionary, int hash_size,  const char * key) {
	unsigned int hashval;
	DNode *np;

	if ((np = lookup (dictionary, hash_size, key)) == NULL ) { //
		np = (DNode *) malloc (sizeof (*np));

		if (np == NULL || (np->key = copystr (key)) == NULL)
			return NULL;

		hashval = hash (key) % hash_size;

		//now links itself on top of array entry
		np->next = dictionary [hashval];
		dictionary [hashval] = np;
	}
	return np;
}

void free_dictionary (DNode ** dictionary, int hash_size) {
	int i;
	for (i=0; i<hash_size; i++) { //iterate over hash array
		if (dictionary [i]!=NULL) { //if there is an entry at position i
			DNode *head = dictionary[i]; //find the head of the linked list
			DNode *current = head;
			while (current != NULL) {
				DNode * temp = current;
				current = current->next;
				free (temp);
			}
			dictionary[i] = NULL;  //BUG fix
		}
	}	 
}

char *copystr(const char *s) { /* make a duplicate of s */
	char *p;
	int len = strlen(s);

	p = (char *) malloc(len+1); /* +1 for ’\0’ */
	if (p != NULL)
		strncpy(p, s, len);
	p[len] = '\0';

	return p;
}