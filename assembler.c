#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "first_pass.h"
#include "first_pass_utility_functions.h"
#include "second_pass.h"



int main(int argc, char *argv[]){

	FILE *input_fp, *output_fp; /*fp: file pointer*/
	char *as_file; /*as_file refers to the source file, that ends with .as*/
	char *am_file; /*am_file refers to the new file generated after macro expantion*/
	machine_code *machine_code_arr;
	labels *head;
 	labels *current;
	
	machine_code_arr = (machine_code *)calloc(MAX_DIGITS * TWO_FIELDS, RAM_SIZE);/*each element in this array has two fields:
 	address, and the instruction/data*/
	if(machine_code_arr == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	head = NULL;
 	current = NULL;
    
    
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
		head = first_pass(output_fp, machine_code_arr, head, current);
		
		/*printf("Label List:\n");
		print_label_list(head);
		printf("\n\n\n\n\n");*/
		
		rewind(output_fp);
		second_pass(output_fp, head);
		
		free(as_file);
		free(am_file);
	}
     
	fclose(output_fp);     
     
	return 0;
}

