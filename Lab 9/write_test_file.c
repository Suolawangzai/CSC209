#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  FILE *fp;
  int i, j;
  if(argc != 2){
    fprintf(stderr, "Need to provide argument of file name \n");
    exit(1);
  }
  char* file_name = argv[1];
  fp = fopen(file_name, "wb");
  if(fp == NULL){
    fprintf(stderr, "Error in opening file \n");
    exit(1);
  }

  for(i = 0; i < 100; i++){
    j = random() % 100;
    fwrite(&j, 1, sizeof(int), fp);
  }

  if(fclose(fp) == -1){
    fprintf(stderr, "Error in closing file \n");
    exit(1);
  }


  fp = fopen(file_name, "rb");
  while(fread(&i, 1, sizeof(int), fp) > 0){
    printf("%d\n", i);
  }
  if(fclose(fp) == -1){
    fprintf(stderr, "Error in closing file \n");
    exit(1);
  }


  return 0;
}
