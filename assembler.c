#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"


int main(int argc, char *argv[]){
	FILE *input_fp, *output_fp; /*fp: file pointer*/
	/*FILE *am_fp;*/
	char *file_name;
	int file_exist;
    
	/*if the user didn't provide any file*/
	if(argc == 1){
		printf("\nerror - there is no files!\n");
		return 0;
	}

	while(--argc > 0){
	
		file_exist = TRUE;
		
		file_name = (char *)malloc(strlen(*argv));
		if(file_name == NULL){
			printf("\nmemory allocation failed, cannot open file.\n");
			exit(1);
		}

		strcpy(file_name, *++argv);
		strcat(file_name, ".as");      
	
		/*open file for reading*/
		if(((input_fp = fopen(file_name, "r")) == NULL) && (argc > 1)){
			printf("\ncannot open file %s, continue with next file.\n\n", *argv);
			file_exist = FALSE;
		}
		
		else if(((input_fp = fopen(file_name, "r")) == NULL) && (argc == 1)){
			printf("\ncannot open file %s.\n\n", *argv);
			exit(1);
		}
	
	
		if(file_exist){
			/*creating a new file with the extention ".am"*/
			strcpy(file_name, strtok(file_name, "."));
			strcat(file_name, ".am");

			/*open file for writing*/
			if(((output_fp = fopen(file_name, "w")) == NULL) && (argc > 1))
				printf("\ncannot open file %s, continue with next file.\n\n", *argv);

			else if(((output_fp = fopen(file_name, "w")) == NULL) && (argc == 1)){
				printf("\ncannot open file %s.\n\n", *argv);
				exit(1);
			}

			/*am_fp = preprocessor(input_fp, output_fp);*/    /*waiting to be used..*/
			/*to be continued*/

		}

	}
     
	free(file_name);
	fclose(output_fp);
	fclose(input_fp);
     
     
return 0;
}

