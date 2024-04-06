#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "data_structures.h"

void first_pass(FILE *input_fp);
int is_empty_line(char *line);
int is_label(char *line, int i);
int is_alphabetic(char character);
int is_command(char *line, int li);


opcode opcodes_table[] = {

	{"mov", "0000", 2, {0,1,2,3,-1}, {1,2,3,-1}},
	{"cmp", "0001", 2, {0,1,2,3,-1}, {0,1,2,3,-1}},
	{"add", "0010", 2, {0,1,2,3,-1}, {1,2,3,-1}},
	{"sub", "0011", 2, {0,1,2,3,-1}, {1,2,3,-1}},
	{"not", "0100", 1, {-1}, {1,2,3,-1}},
	{"clr", "0101", 1, {-1}, {1,2,3,-1}},
	{"lea", "0110", 2, {1,2,-1}, {1,2,3,-1}},
	{"inc", "0111", 1, {-1}, {1,2,3,-1}},
	{"dec", "1000", 1, {-1}, {1,2,3,-1}},
	{"jmp", "1001", 1, {-1}, {1,2,3,-1}},
	{"bne", "1010", 1, {-1}, {1,2,3,-1}},
	{"get", "1011", 1, {-1}, {1,2,3,-1}},
	{"prn", "1100", 1, {-1}, {0,1,2,3,-1}},
	{"jsr", "1101", 1, {-1}, {1,2,3,-1}},
	{"rts", "1110", 0, {-1}, {-1}},
	{"hlt", "1111", 0, {-1}, {-1}}

};







int main(){
	
	FILE *input_file;
    input_file = fopen("test_output.am", "r");  

    if (input_file == NULL) {
        perror("Error opening files");
        return 1;
    }
    
    first_pass(input_file);
    
    fclose(input_file);
	
	return 0;
}






void first_pass(FILE *input_fp){
	
	char *line, *word;
 	int /*IC, DC,*/ i, line_num;
 	
  
	/*IC = 0;
	DC = 0;*/
	i = 0;
	line_num = 1;
  
	line = (char *)calloc(sizeof(char), LINE_SIZE);
	word = (char *)calloc(sizeof(char), LINE_SIZE);
	if(line == NULL && word == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	fgets(line, LINE_SIZE, input_fp);
	
	while(!feof(input_fp)){
		
		if(is_empty_line(line) || line[0] == ';'){/*if line is empty or it's a comment*/
			fgets(line, LINE_SIZE, input_fp);
			line_num++;
		}
		
		else/* if()*/{
			
			while(line[i] == ' ' || line[i] == '\t'){/*skipping spaces*/
				i++;
			}
			
			if(is_label(line, i)){
				printf("%d: It's a label!!\n", line_num);
			}
			
			else if(is_command(line, i)){
				/*printf("%d: It's a command!! ", line_num);*/
			}
			
		}
		
		fgets(line, LINE_SIZE, input_fp);
		line_num++;
		i = 0;
	}
	
}






/*returns true if parameter line has only blanks*/
int is_empty_line(char *line){
	
	int i;
	i = 0;
	
	while(line[i] == ' ' || line[i] == '\t'){
		i++;
	}
	
	if(line[i] == '\n'){
		return TRUE;
	}
	
	return FALSE;
}






int is_label(char *line, int i){
	
	char character;
	
	character = line[i];
	
	/*if character is a letter*/
	if(!is_alphabetic(character)){
		return FALSE;
	}
	
	i++;
	while(i <= 30 && line[i] != ':' && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF){
		if(!is_alphabetic(line[i]) && (line[i] < 48 || line[i] > 57)){
			return FALSE;
		}
		i++;
	}
	
	if(line[i] != ':'){
		return FALSE;
	}
	
	return TRUE;
}






int is_alphabetic(char character){
    
	if((character >= 65 && character <= 90) || (character >= 97 && character <= 122))
		return TRUE;
  
	else return FALSE;
}





int is_command(char *line, int li){
	
	int i, wi;
	char word[LINE_SIZE];
	
	i = 0;/*index for opcodes_table*/
	wi = 0;/*wi = word index*/
	
	while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
		word[wi] = line[li];
		wi++;
		li++;
	}
	word[wi] = '\0';
	
	for(i = 0 ; i < COMMAND_QTY ; i++){
		if(strcmp(word, opcodes_table[i].command) == 0){
			printf("%s\n", opcodes_table[i].command);
			return TRUE;
		}
	}
	return FALSE;
}













