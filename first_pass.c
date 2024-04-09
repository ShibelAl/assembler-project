#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include "defines.h"*/
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
	
	char *line;
 	int IC, DC, line_num, error;

 	
	IC = 0; /* Instruction Counter */
	DC = 0; /* Data Counter */
	line_num = 1;
	error = FALSE; 
  
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
			line_decode(line, line_num, &error, &DC, &IC);
		}
		
		fgets(line, LINE_SIZE, input_fp);
		line_num++;
	}
	
}





void line_decode(char *line, int line_num, int *error, int *DC, int *IC){
	
	char type, word[LINE_SIZE];
	int i, wi; /*wi = word index*/
	labels *head, *current;
	
 	head = NULL;
 	current = NULL;
 	i = 0;
	wi = 0;
	
	while(line[i] != '\n' && line[i] != EOF){ /* line decode */
				
		while(line[i] == ' ' || line[i] == '\t'){/* skip spaces */
			i++;
		}
	
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF){
			word[wi] = line[i];
			wi++;
			i++;
		}
		word[wi] = '\0';
		wi = 0;
		
		printf("------------%s-------------\n", word);
		
		if(is_label(word)){
		printf("%d: It's a label!!\n", line_num);
			word[strlen(word) - 1] = '\0';/*removing the ':' from the label*/
			if(is_name_in_list(head, word)){
				printf("%d:\tError - Multiple declarations of the same label.\n", line_num);
				*error = TRUE;
			}
			type = line_type(line, i, line_num, error);
			if(type == 'd'){
				append_label_node(&head, &current, word, *DC, FALSE, FALSE, FALSE);
			}
			else if(type == 'i'){
				append_label_node(&head, &current, word, *IC, FALSE, FALSE, TRUE);
			}
			else if(type == 'x'){/*there is an error*/
				return;
			}
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
		
		else if(strcmp(word, ".entry") == 0){
			printf("%d: It's .struct!!\n", line_num);
		}
		
		else if(strcmp(word, ".extern") == 0){
			printf("%d: It's .struct!!\n", line_num);
		}
				
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





/*
* Parameters:
* line: current line in the file as array of characters.
* i: index for traversing line.
* line_num: number of the current line in the file.
* error: indicator for error existing in line, error = TRUE if line has an error.
* 
* Returns:
* d: if the current line is a directive statement.
* i: if the current line is an instruction statement.
* x: if the current line is neither directive nor instruction statement.
*/

char line_type(char *line, int i, int line_num, int *error){
	
	char word[LINE_SIZE];
	int wi;
	wi = 0;
	
	while(line[i] == ' ' || line[i] == '\t'){
		i++;
	}
	
	while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF){
		word[wi] = line[i];
		wi++;
		i++;
	}
	word[wi] = '\0';
	
	if(word[0] == '.'){/*if word is a start of a directive statement*/
		if(strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0 || strcmp(word, ".struct") == 0){
			return 'd';
		}
		
		/*if(strcmp(word, ".entry") == 0 || strcmp(word, ".extern") == 0){
			return 'e';
		}*/
		
		else{
			printf("Error (line %d) - illegal directive statement\n", line_num);
			*error = TRUE;
			return 'x';
		}
	}
	
	else if(is_command(word)){
		return 'i';
	}
	
	else{/*line can't be neither directive not instruction statement*/
		printf("Error (line %d) - illegal statement\n", line_num);
		*error = TRUE;
		return 'x';
	} 
}














