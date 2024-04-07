#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "data_structures.h"
#include "first_pass.h"


/*opcode opcodes_table[] = {

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
}*/





/*This function receives pointer to the assembly language file after macro handling, 
it passes on the file, saves all the labels in a label table, and saves the addresses in binary
of some, currently I decided to not return anything*/
void first_pass(FILE *input_fp){
	
	char *line, word[LINE_SIZE];
 	int /*IC, DC,*/ i, wi, line_num;
 	
	/*IC = 0;
	DC = 0;*/
	i = 0; /*index for line*/
	wi = 0; /*wi = word index*/
	line_num = 1;
  
	line = (char *)calloc(sizeof(char), LINE_SIZE);
	if(line == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	fgets(line, LINE_SIZE, input_fp);
	
	while(!feof(input_fp)){
		
		if(is_empty_line(line) || line[0] == ';'){/*if line is empty or it's a comment*/
			fgets(line, LINE_SIZE, input_fp);
			line_num++;
		}
		
		else{
			
			while(line[i] == ' ' || line[i] == '\t'){/*skipping spaces*/
				i++;
			}
			
			/*word contains the first word in the line*/
			while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF){
				word[wi] = line[i];
				wi++;
				i++;
			}
			word[wi] = '\0';
			wi = 0;
			i = 0;
		
			if(is_label(word)){
				
				printf("%d: It's a label!!\n", line_num);
			}
			
			else if(is_command(word)){
				printf("%d: It's a command!!\n", line_num);
			}
			
			else if(strcmp(word, ".data") == 0){
				printf("%d: It's .data!!\n", line_num);
			}
			
			else if(strcmp(word, ".string") == 0){
				printf("%d: It's .string!!\n", line_num);
			}
			
			else if(strcmp(word, ".struct") == 0){
				printf("%d: It's .struct!!\n", line_num);
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





/*returns true if parameter word is a label, false otherwise*/
int is_label(char *word){
	
	int wi; /*wi = word index*/
	wi = 0;
	
	/*if the first character is not a letter, word is not a label*/
	if(!is_alphabetic(word[0])){
		return FALSE;
	}
	
	while(word[wi] != '\0' && word[wi] != ':'){
		if(!is_alphabetic(word[wi]) && (word[wi] < 48 || word[wi] > 57)){
			return FALSE;
		}
		wi++;
	}

	if(word[wi] != ':'){ /*label should end with a ':'*/
		return FALSE;
	}
	
	return TRUE;
}





/*returns true if parameter character is an alphabet letter, false otherwise*/
int is_alphabetic(char character){
    
	if((character >= 65 && character <= 90) || (character >= 97 && character <= 122))
		return TRUE;
  
	else return FALSE;
}




/*returns true if parameter word is a command from the 16 commands
of the language, false otherwise*/
int is_command(char *word){
	
	int i; /*index for opcodes_table*/
	i = 0; 
	
	for(i = 0 ; i < COMMAND_QTY ; i++){
		if(strcmp(word, opcodes_table[i].command) == 0){
			return TRUE;
		}
	}
	return FALSE;
}






















