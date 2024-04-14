#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"
#include "first_pass.h"



/*int main(){
	
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
			if(error == TRUE){
				error = FALSE;
			}
		}
		
		fgets(line, LINE_SIZE, input_fp);
		line_num++;
	}
	
}



	



/*Parameters:
* line: the current line in the file.
* line_ num: number of the line in the file.
* error: flag for error existence, TRUE if there is an error, false otherwise. 
* DC: stands for Data Counter.
* IC: stands for Instruction Counter.
* 
* Goal:
* reading every line in a file, and for each line, if it conteins an error, the parameter
* error turns to TRUE and a dedicated message printed to the user for each error,
* if everything is ok, it will decide whether the line is a directive or an instruction
* statement, for each kind, it will send the line to an appropreate function in order to 
* load the code written in the file, to the memory. 
*/
void line_decode(char *line, int line_num, int *error, int *DC, int *IC){
	
	char type, word[LINE_SIZE];
	int i, wi; /*wi = word index*/
	labels *head, *current;
	
 	head = NULL;
 	current = NULL;
 	i = 0;
	wi = 0;
	
	/*syntax_errors(line, line_num, error);*/
	/*temporarely, for now let's assume there isn't any syntax errors..*/
	
	while(line[i] != '\n' && line[i] != EOF){ /* line decode */
				
		while(line[i] == ' ' || line[i] == '\t' || line[i] == ','){/* skip spaces */
			i++;
		}
		
		/*take the first word of the line*/
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF && line[i] != ','){
			word[wi] = line[i];
			wi++;
			i++;
		}
		word[wi] = '\0';
		wi = 0;
		
		printf("------------%s-------------\n", word);
		
		if(is_label(word)){
		/*printf("%d: It's a label!!\n", line_num);*/
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
				/**IC++;*/
			}
			else if(type == 'x'){/*there is an error*/
				return;
			}
		}
		
		else if(is_command(word)){
			/*printf("%d: It's a command!!\n", line_num);*/
			store_instruction_line(line/*, i*/);
			
			
		}
				
		else if(strcmp(word, ".data") == 0){
			/*printf("%d: It's .data!!\n", line_num);*/
		}
				
		else if(strcmp(word, ".string") == 0){
			/*printf("%d: It's .string!!\n", line_num);*/
		}
				
		else if(strcmp(word, ".struct") == 0){
			/*printf("%d: It's .struct!!\n", line_num);*/
		}
		
		else if(strcmp(word, ".entry") == 0){
			/*printf("%d: It's .struct!!\n", line_num);*/
		}
		
		else if(strcmp(word, ".extern") == 0){
			/*printf("%d: It's .struct!!\n", line_num);*/
		}
				
	}
			
}





/*Parameters:
* line: current line in the file.
* i: index for traversing line.
* 
* Goal: 
* this function reads the line, that was previously known to be an instruction statement,
* and it stores the number of the command and addresses of some of the operands in binary
* in a dedicated table called .. (will name it something..)
*/
void store_instruction_line(char *line/*, int i*/){
	
	char word[LINE_SIZE];
	/*char binary_code[11];*/ /*a word in the memory is stored via 10 bits*/
	int wi, i;
	wi = 0;
	i = 0;
	
	store_line_opcode(line);
	
	
	while(line[i] == ' ' || line[i] == '\t'){
		i++;
	}
	
	while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != EOF){
		word[wi] = line[i];
		wi++;
		i++;
	}
	word[wi] = '\0';
	
	if(word[0] == '#'){/*Immediate addressing*/
		/*strcat(binary_code, );*/
		printf("Immediate addressing\n");
	}
	
	else if(is_register(word)){/*Direct register addressing*/
		printf("Direct register addressing\n");
	}
	
	else if(operand_is_label(word)){/*Direct addressing*/
		printf("Direct addressing\n");
	}
	
	else{/*Accessing struct addressing*/
		printf("Accessing struct addressing\n");
	}
	
}






/*
* this function stores opcode of the current line. 
* I use this function only for storing the command binary code, 
* source and destination addressing type, and the A,R,E field.
*/
void store_line_opcode(char *line){
	
	char word[LINE_SIZE];
	int wi, li, oi;
	wi = 0;/*wi = word index*/
	li = 0;/*li = line index*/
	oi = 0;/*oi = opcodes table index*/
	
	while(line[li] == ' ' || line[li] == '\t'){
		li++;
	}
	
	while(line[li] != ' ' && line[li] != '\t' && line[li] != '\n' && line[li] != EOF){
		word[wi] = line[li];
		wi++;
		li++;
	}
	word[wi] = '\0';
	
	for(oi = 0 ; oi < COMMAND_QTY ; oi++){
		if(strcmp(word, opcodes_table[oi].command) == 0){
			
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






/*returns true if the parameter word is a name of a register*/
int is_register(char *word){
	
	int i;
	
	for(i = 0 ; i < REGISTER_QTY ; i++){
		if(strcmp(word, registers_table[i].register_name) == 0){
			return TRUE;
			printf("It's a register!!");
		}
	}
	return FALSE;
}






/*returns true if parameter word is a label as an operand in an instruction statement*/
int operand_is_label(char *word){
	
	int wi;/*wi = word index*/
	
	if(!is_alphabetic(word[0])){
		return FALSE;
	}
	
	for(wi = 1 ; word[wi] != '\0' ; wi++){
		/*if the current character isn't a letter or a number, return false*/
		if(!is_alphabetic(word[wi]) && (word[wi] < 48 || word[wi] > 57)){
			return FALSE;
		}
	}
	return TRUE;
}







/*Parameters:
* decimal: integer number.
* 
* Returns:
* the number decimal in base 32.
*/
char *decimal_to_base32(int decimal){
	
	int index, i, j;/* i, j is for reversing a string */
    char *base32_representation, temp;
    char base32_symbols[] = "!@#$%^&*<>abcdefghijklmnopqrstuv";
    
    base32_representation = (char *)malloc(MAX_DIGITS * sizeof(char));
    if(base32_representation == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }

    do{
        base32_representation[index] = base32_symbols[decimal % 32];
        decimal /= 32;
        index++;
    } while(decimal != 0);
    base32_representation[index] = '\0';

    /* Reverse the string */
    for (i = 0, j = index - 1; i < j; i++, j--){
        temp = base32_representation[i];
        base32_representation[i] = base32_representation[j];
        base32_representation[j] = temp;
    }

    return base32_representation;
}






/*Parameters:
* binary_str: a string that represents a binary number.
* 
* Returns:
* the binary number in base 32.
*/
char *binary_to_base32(char *binary_str){

    char *base32_representation, binary_segment[6];
	char base32_symbols[] = "!@#$%^&*<>abcdefghijklmnopqrstuv";
	int index, i, decimal;
	
	base32_representation = (char *)malloc(MAX_DIGITS * sizeof(char));
    if (base32_representation == NULL){
		printf("Memory allocation failed\n");
        return NULL;
	}
	
	index = 0;
	
    for (i = 0; i < BINARY_LENGTH; i += 5){
         /* Extract 4-digit binary segment */
        strncpy(binary_segment, binary_str + i, 5);
        binary_segment[6] = '\0'; 

        /* Convert binary segment to decimal */
        decimal = strtol(binary_segment, NULL, 2);

        /* Map decimal to corresponding base32 symbol */
        base32_representation[index] = base32_symbols[decimal];
        index++;
    }
    base32_representation[index] = '\0';
    
    return base32_representation;
}







