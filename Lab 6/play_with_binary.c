#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	int i = 12;
	FILE *data_file, *name_file;
	int error;
	int byte, value = 0;

	if(argc > 1){
		if(!strcmp(argv[1], "b")){
			data_file = fopen("outputFile", "wb");
			if (data_file == NULL) {
        		fprintf(stderr, "Error: could not open file\n");
        		return 1;
    		}
			error = fwrite(&i, sizeof(int), 1, data_file);
			if (error != 1) {
        		fprintf(stderr, "Error: array not fully written to file\n");
        		return 1;
    		}
		}
		else{
			fprintf(stderr, "Error, the argument given is not correct, type b to write binary file \n");
			return 1;
		}
	}
	else{
		data_file = fopen("outputFile", "w");
		if (data_file == NULL) {
        fprintf(stderr, "Error: could not open file\n");
        return 1;
    	}
    	fprintf(data_file, "%d\n", i);
	}

	error = fclose(data_file);
    if (error != 0) {
        fprintf(stderr, "Error: fclose failed\n");
        return 1;
    }


	name_file = fopen("nameFile", "w");
	if (name_file == NULL) {
        fprintf(stderr, "Error opening output file\n");
        return 1;
    }
  
	fprintf(name_file, "Meng Zhang");

	error = fclose(name_file);
    if (error != 0) {
        fprintf(stderr, "fclose failed on input file\n");
        return 1;
    }


    name_file = fopen("nameFile", "rb");
    if (name_file == NULL) {
        fprintf(stderr, "Error: could not open file\n");
        return 1;
    }

int read = 0;
while((read = fread(&byte, sizeof(char), 4, name_file)) > 0){
	value += byte;
    fprintf(stdout, "%d \t", byte);
}
fprintf(stdout, "\n");
fprintf(stdout, "Your name has value of %d \n", value);

error = fclose(name_file);
if (error != 0) {
    fprintf(stderr, "fclose failed on input file\n");
    return 1;
}

	return 0;

}