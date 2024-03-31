#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"


FILE* store_macro(FILE *input_fp, FILE *output_fp);

typedef struct macro{
	
	char *macro_name;
	char *macro_content;
	
}macro;



FILE* store_macro(FILE *input_fp, FILE *output_fp){
	
	char *line;
	char *word; /*in order to read one word if needed, and store macro name*/
	char *content; /*in order to store macro content*/
	macro *macro_arr;/*an array of macro name and macro content*/
	int li;/*li = line index*/
	int wi;/*wi = word index*/
	int ci;/*ci = content index*/
	int mi;/*mi = macro_arr index*/
	
	li = 0;
	wi = 0;
	ci = 0;
	mi = 0;
	
	
	line = calloc(sizeof(int), LINE_SIZE);
	word = calloc(sizeof(int), LINE_SIZE);
	content = calloc(sizeof(int), LINE_SIZE);
	macro_arr = (macro *)calloc(sizeof(macro), 2);
	if(line == NULL || word == NULL || content == NULL || macro_arr == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	
	/*Here I will begin reading the assembly file line by line*/
	
	
	/*currently I don't want to free macro_arr, will see!*/
	free(line);
	free(word);
	free(content);
	return output_fp;
}

