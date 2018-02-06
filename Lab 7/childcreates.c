#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

	int i, val;
	int n;
	int num_kids;
	if(argc != 2) {
		fprintf(stderr, "Usage: forkloop <numkids>\n");
		exit(1);
	}

	num_kids = atoi(argv[1]);

	for(i = 0; i < num_kids; i++) {

		if(i == 0){
			n = fork();
		    if(n < 0) {
				perror("fork");
				exit(1);
			}
		}
		else{
			if(n > 0){
				n = fork();
				if(n < 0) {
					perror("fork");
					exit(1);
				}
			}
		}

 		
 		printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
	}

	wait(&val);
	return 0;
}