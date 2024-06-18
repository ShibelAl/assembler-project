#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "first_pass.h"



int main(int argc, char *argv[]){

	FILE *input_fp, *output_fp; /*fp: file pointer*/
	char *as_file; /*as_file refers to the source file, that ends with .as*/
	char *am_file; /*am_file refers to the new file generated after macro expantion*/
    /*labels *head;
 	labels *current;
 	
 	head = NULL;
 	current = NULL;*/
    
    
    
	/*if the user didn't provide any file*/
	if(argc == 1){
		printf("error - there is no files!\n");
		return 0;
	}
	
	while(--argc > 0){
		
		argv++;
		
		as_file = (char *)malloc(strlen(*argv));
		am_file = (char *)malloc(strlen(*argv));
		if(as_file == NULL || am_file == NULL){
			printf("memory allocation failed, cannot open file.\n");
			exit(1);
		}
		
		strcpy(as_file, *argv);
		strcpy(am_file, *argv);
		
		strcat(as_file, ".as");
		strcat(am_file, ".am");
		
		input_fp = fopen(as_file, "r");
		output_fp = fopen(am_file, "w+");
		
		if(input_fp == NULL){
			printf("cannot open file %s.\n", as_file);
			continue;	
		}
		
		if(output_fp == NULL){
			printf("cannot open file %s.\n", am_file);
			continue;
		}
		
		expand_macro(input_fp, output_fp);
		fclose(input_fp);
		
		rewind(output_fp);
		first_pass(output_fp);
		
		free(as_file);
		free(am_file);
	}
     
	fclose(output_fp);     
     
	return 0;
}

