#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"


FILE* store_macro(FILE *input_fp, FILE *output_fp);
int is_macro(char *line);
int is_macro_name();

typedef struct macro{
	
	char *macro_name;
	char *macro_content;
	
}macro;


/*This function receives two pointers to files
input_fp contains the cource code, whereas output_fp is the new file after handling the macros.
This function returns a pointer to file (output_fp) that is after macro handling.*/
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
	
	fgets(line, LINE_SIZE, input_fp);
	
	while(!feof(input_fp)){
		if(is_macro(line)){
			printf("store the macro");/*temporary printf*/
		}
		else if(is_macro_name(/*something*/)){
			printf("put the macro related to this name");/*temporary printf*/
		}
		else{
			fputs(line, output_fp);
		}
	}
	
	
	/*currently I don't want to free macro_arr, will see!*/
	free(line);
	free(word);
	free(content);
	return output_fp;
}



/*This function checks if the first word in the line "line" is a macro name that
has been declared and stored previously in "macro_arr"*/
int is_macro(char *line){
	
	int li;/*li = line index*/
	char word[LINE_SIZE];
	int wi;/*wi = word index*/
	li = 0;
	wi = 0;
	
	while(line[li] == ' ' || line[li] == '\t'){/*skipping spaces*/
		li++;
	}
	if(line[li] == '\n' || line[li] == EOF){/*there is no macro*/
		return FALSE;
	}
	/*if the function didn't return FALSE, then that's a word, saving it:*/
	while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
		word[wi] = line[li];
		wi++;
		li++;
	}
	word[wi] = '\0';
	if(strcmp(word, "macro") == 0){/*if the word is macro*/
		return TRUE;
	}
	else{
		return FALSE;
	}
}



/*function need to be implemented.
It checks if the word we are reading from the source file is macro name.*/
int is_macro_name(){
	return TRUE;
}

