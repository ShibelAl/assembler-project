#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "preprocessor.h"



/*This function receives two pointers to files
input_fp contains the cource code, whereas output_fp is the new file after handling the macros.
This function returns a pointer to file (output_fp) that is after macro handling.*/
FILE* store_macro(FILE *input_fp, FILE *output_fp){
	
	char *line, *word;
	macro *macro_arr;/*an array of macro name and macro content*/
	int mi, li, wi, count;
	/*mi = macro_arr index, li = line index, wi = word index, count is for realloc*/
	
	li = 0;
	wi = 0;
	mi = 0;
	count = 2; /*to increase memory, so count should be >= 2*/
	
	
	line = calloc(sizeof(int), LINE_SIZE);
	word = calloc(sizeof(int), LINE_SIZE);
	macro_arr = (macro *)calloc(sizeof(macro), 2);
	if(line == NULL || word == NULL || macro_arr == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	fgets(line, LINE_SIZE, input_fp);
	
	while(!feof(input_fp)){
	
		if(is_macro(line)){
		
			/*## start storing macro name ##*/
			macro_arr[mi].macro_name = (char *)malloc(LINE_SIZE);
			macro_arr[mi].macro_content = (char *)malloc(LINE_SIZE);
			if(macro_arr[mi].macro_name == NULL || macro_arr[mi].macro_content == NULL){
				printf("\nmemory allocation failed\n");
				exit(1);
			}
			
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
			if(strcmp(word, "macro") == 0){/*I want to store macro name not the word macro*/
				
				for(wi = 0 ; wi < LINE_SIZE ; wi++){
					word[wi] = '\0';
				}
				wi = 0;
				
			}
			while(line[li] == ' ' || line[li] == '\t'){/*skipping spaces*/
				li++;
			}
			while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
				word[wi] = line[li];
				wi++;
				li++;
			}
			strcpy(macro_arr[mi].macro_name, word);
			
			for(wi = 0 ; wi < LINE_SIZE ; wi++){
				word[wi] = '\0';
			}
			wi = 0;
			
			/*## end of storing macro name ##*/
			/*## start storing macro content ##*/
			
			li = 0;
			fgets(line, LINE_SIZE, input_fp);
			
			while(strcmp(word, "endmacro") != 0){
				
				strcat(macro_arr[mi].macro_content, line);
				macro_arr[mi].macro_content = (char *)realloc(macro_arr[mi].macro_content, LINE_SIZE * count);
				if(macro_arr[mi].macro_content == NULL){
					printf("\nmemory allocation failed\n");
					exit(1);
				}
				count++;
				
				fgets(line, LINE_SIZE, input_fp);
				
				while(line[li] == ' ' || line[li] == '\t'){/*skipping spaces*/
					li++;
				}
				/*if the function didn't return FALSE, then that's a word, saving it:*/
				while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
					word[wi] = line[li];
					wi++;
					li++;
				}
				
				wi = 0;
				li = 0;	
			}
			
			for(wi = 0 ; wi < LINE_SIZE ; wi++){
				word[wi] = '\0';
			}
			wi = 0;

			mi++;
			/*## end of storing macro content ##*/
		}
	
		
		else if(is_macro_name(line, macro_arr)){
			put_macro_content(line, macro_arr, output_fp);/*temporary printf*/
		}
		
		else{
			fputs(line, output_fp);
		}
		
	}
	free(line);
	free(word);
	return output_fp;
}





/*This function checks if the first word in the line "line" is a macro name that
has been declared and stored previously in "macro_arr"*/
int is_macro(char *line){
	
	int li, wi;/*li = line index, wi = word index*/
	char word[LINE_SIZE];
	
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





/*This function checks if the first word in the line "line" is a macro name that
has been declared and stored previously in "macro_arr"*/
int is_macro_name(char *line, macro *macro_arr){
	
	int li, wi, mi;/*li = line index, wi = word index, mi = macro_arr index*/
	char word[LINE_SIZE];
	
	li = 0;
	wi = 0;
	mi = 0;
	
	if(macro_arr == NULL){
		return FALSE;
	}
	
	while(line[li] == ' ' || line[li] == '\t'){/*skipping spaces*/
		li++;
	}
	if(line[li] == '\n' || line[li] == EOF){/*blank line*/
		return FALSE;
	}
	/*if the function didn't return FALSE, then that's a word, saving it:*/
	while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
		word[wi] = line[li];
		wi++;
		li++;
	}
	word[wi] = '\0';
	while(macro_arr[mi].macro_name != NULL){
		if(strcmp(word, macro_arr[mi].macro_name) == 0){/*if the word is macro*/
			return TRUE;
		}
		mi++;
	}
	
	return FALSE;
	
}





/*This function receives one line, the array of macros "macro_arr", and 
a pointer to after-macro file "output_fp".
It writes the macro content in output_fp.*/
void put_macro_content(char *line, macro *macro_arr, FILE *output_fp){
	
	int li, wi, mi;/*li = line index, wi = word index, mi = macro_arr index*/
	char word[LINE_SIZE];
	
	li = 0;
	wi = 0;
	mi = 0;
	
	while(line[li] == ' ' || line[li] == '\t'){/*skipping spaces*/
		li++;
	}
	
	while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
		word[wi] = line[li];
		wi++;
		li++;
	}
	word[wi] = '\0';
	while(macro_arr[mi].macro_name != NULL && strcmp(word, macro_arr[mi].macro_name) != 0){
		mi++;
	}
	
	fputs(macro_arr[mi].macro_content, output_fp);
	printf("i'm in put_macro_content: %d\n", mi);
	
}


