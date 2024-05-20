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
 	int IC, DC, line_num, error, mi;
 	machine_code *machine_code_arr;
	
 	
	IC = 100; /* Instruction Counter */
	DC = 0; /* Data Counter */
	line_num = 1;
	mi = 0; /*mi = machine_code_array index*/
	error = FALSE; 
	
	
	
	line = (char *)calloc(sizeof(char), LINE_SIZE);
	machine_code_arr = (machine_code *)calloc(MAX_DIGITS * TWO_FIELDS, RAM_SIZE);/*each element in this array has two fields:
 	address, and the instruction/data*/
	if(line == NULL || machine_code_arr == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	fgets(line, LINE_SIZE, input_fp);
	
	while(!feof(input_fp)){
		
		if(!is_empty_line(line) && line[0] != ';'){/*if line is not empty nor it's a comment*/
			/*memory allocation for the current element*/
			/*machine_code_arr[mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
			machine_code_arr[mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
			if(machine_code_arr[mi].address == NULL || machine_code_arr[mi].code == NULL){
				printf("\nmemory allocation failed\n");
				exit(1);
			}*/
			
			line_decode(line, line_num, machine_code_arr, &mi, &error, &DC, &IC);
			/*mi++;*/
			if(error == TRUE){
			printf("\nError!!\n");
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
void line_decode(char *line, int line_num, machine_code *machine_code_arr, int *mi, int *error, int *DC, int *IC){
	
	char type, word[LINE_SIZE];
	int i, wi; /*wi = word index*/
	labels *head, *current;
	
 	head = NULL;
 	current = NULL;
 	i = 0;
	wi = 0;
	
	/*syntax_errors(line, line_num, error);*/
	/*temporarely, for now let's assume there isn't any syntax errors..*/
	
	while(line[i] != '\n' && line[i] != EOF){
		
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
		
		/*printf("%s ", word);*/
		/*printf("we are at %d \n", mi);*/
		
		if(is_label(word)){
	
			word[strlen(word) - 1] = '\0';/*removing the ':' from the label*/
			if(is_name_in_list(head, word)){
				printf("%d:\tError - Multiple declarations of the same label.\n", line_num);
				*error = TRUE;
			}
			type = line_type(line, i, line_num, error);
			if(type == 'd'){/*if the current line is a directive statement.*/
				append_label_node(&head, &current, word, *DC, FALSE, FALSE, FALSE);
			}
			else if(type == 'i'){/*if the current line is an instruction statement.*/
				append_label_node(&head, &current, word, *IC, FALSE, FALSE, TRUE);
			}
			else if(type == 'x'){/*there is an error*/
				*error = TRUE;
				return;
			}
		
		}
		
		else if(is_command(word)){
			/*printf("%d: It's a command!!\n", line_num);*/
			store_instruction_line(line, i, machine_code_arr, mi, word, IC);/*storing the address of the command,
			and the addressing methods code that is set according to each operand*/
			*mi = *mi + 1;
			store_instruction_line_operands(line, i, machine_code_arr, mi, &head, IC);
			/**IC++;*/
			break;
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
		
		while(wi < LINE_SIZE && word[wi] != '\0'){/*delete word*/
			word[wi] = '\0';
		}
		wi = 0;
		
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
* 
* NOTE: the parameter i starts after reading the command, so the function receives the command as parameter.
*/
void store_instruction_line(char *line, int i, machine_code *machine_code_arr, int *mi, char *command, int *IC){
	
	char word[LINE_SIZE];
	char opcode[11];
	char addressing_method[5];/*for storing addressing method binary codes*/
	char *decimal_base32, *binary_base32;
	int wi, oi, has_register;
	int ai;
	wi = 0; /*wi = word index*/
	oi = 0; /*oi = opcodes table index.*/
	ai = 0;
	has_register = FALSE;/*I made this flag because if the statement has a 
	command and two registers, IC doesn't increase for the second register*/
	
	machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	
	/*saving the command binary code*/
	for(; oi < COMMAND_QTY ; oi++){
		if(strcmp(command, opcodes_table[oi].command) == 0){
		
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);		
			free(decimal_base32);
			*IC = *IC + 1;
			strcpy(opcode, opcodes_table[oi].in_binary);/*--put the first 4 bits in the opcode--*/
		}
	}
	
	
	
	while(line[i] != '\n' && line[i] != EOF){
		
		while(line[i] == ' ' || line[i] == '\t' || line[i] == ','){/*skip spaces*/
			i++;
		}
		
		/*take the next word*/
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != ',' && line[i] != EOF){
			word[wi] = line[i];
			wi++;
			i++;
		}
		word[wi] = '\0';
		wi = 0;
		
		*IC = *IC + 1;/*new instruction operand, so new IC.*/
	
		/*I arranged these if statements like that for a reason
		(for the similarity between the direct addressing and accessing struct addressing)*/
		if(word[0] == '#'){/*Immediate addressing*/
			strcat(addressing_method, "00");/*00 is the binary code for immediate addressing*/
			/*printf("Immediate addressing  ");*/
		}
		
		else if(is_register(word)){/*Direct register addressing*/
			
			strcat(addressing_method, "11");/*11 is the binary code for register addressing*/
			if(has_register){
				*IC = *IC - 1;/*to prevent increasing the IC for both registers*/
				has_register = FALSE;/*I passed two registers, now reset the flag.*/
			}
			has_register = TRUE;
			/*printf("Direct register addressing  ");*/
		}
		
		else if(operand_is_label(word)){/*Direct addressing*/
			strcat(addressing_method, "01");/*01 is the binary code for direct addressing*/
			/*printf("Direct addressing  ");*/
		}
		
		else{/*Accessing struct addressing*/
			strcat(addressing_method, "10");/*10 is the binary code for accessing struct addressing*/
			*IC = *IC + 1;/*to access the struct field*/
			/*printf("Accessing struct addressing  ");*/
		}
		
	}
	/*because if there is only one operand, the addressing code will be 00 and something*/
	if(strlen(addressing_method) == 2){
		strcat(opcode, "00");
		strcat(opcode, addressing_method);
	}
	else if(strlen(addressing_method) == 0){
		strcat(opcode, "0000");
	}	
	else strcat(opcode, addressing_method);
	
	/*the A.R.E field is 00 when it's an instruction line*/
	strcat(opcode, "00");
	
	/*printf("%s ", opcode);*/
	binary_base32 = binary_to_base32(opcode);
	strcpy(machine_code_arr[*mi].code, binary_base32);
	
	free(binary_base32);
	
	printf("%s %s\n\n", machine_code_arr[*mi].address, machine_code_arr[*mi].code);
	
	/*reset addressing_method and opcode*/
	while(ai < 6){
		addressing_method[ai] = '\0';
		ai++;
	}
	ai = 0;
	while(ai < 12){
		opcode[ai] = '\0';
		ai++;
	}
	ai = 0;
}






/*
* Parameters: 
* line: the current line in the assembly code.
* i: index for traversing line.
* machine_code_arr: the current node in the machine code linked list.
* mi: machine_code_arr index.
* head: the head node in the labels linked list.
* IC: instrction counter.
* 
* Goal: this function stores opcode of the current line; 
* I use this function only for storing the command binary code, 
* source and destination addressing type, and the A,R,E field.
*   
* NOTE: i starts with the first character after the command, for inctance:
* line = MAIN:  mov s1.1 ,LENGTH ---> this function starts with the first space after mov.
*/ 
void store_instruction_line_operands(char *line, int i, machine_code *machine_code_arr, int *mi, labels **head, int *IC){
	
	char word[LINE_SIZE];
	char operand[LINE_SIZE];
	char binary[BINARY_LENGTH];
	char *decimal_base32, *binary_base32;
	int wi, oi;
	wi = 0;
	oi = 0;
	
	/*maybe I should put that inside of the loop so I can allocate memory for each word*/
	/*machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}*/ 
	while(line[i] != '\n' && line[i] != EOF){
		
		while(line[i] == ' ' || line[i] == '\t' || line[i] == ','){
			i++;
		}
		machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
		if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL){
			printf("\nmemory allocation failed\n");
			exit(1);
		}
		
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != ',' && line[i] != EOF){/*saving the operand in word*/
			word[wi] = line[i];
			wi++;
			i++;
		}
		word[wi] = '\0';
		wi = 0;
			
		if(word[0] == '#'){/*Immediate addressing*/
			wi++;/*skip the #*/
			while(word[wi] != '\0'){/*putting the actual number in first operand*/
				operand[oi] = word[wi];
				wi++;
				oi++;
			}
			operand[oi] = '\0';
			oi = 0;
			
			/*&&&&&&&&&&&&&&&&&&&&&&&& HERE IS THE PROBLEM, WITH THE IC. &&&&&&&&&&&&&&&&&&&&&&&*/
			decimal_base32 = decimal_to_base32(*IC - 1);
			strcpy(machine_code_arr[*mi].address, decimal_base32);	
			free(decimal_base32);
			
			strcpy(binary, strcat(int_to_binary(atoi(operand)), "00"));
			binary_base32 = binary_to_base32(binary);
			strcpy(machine_code_arr[*mi].code, binary_base32);
			printf("%s %s\n\n", machine_code_arr[*mi].address, machine_code_arr[*mi].code);
			printf("The number is: %d\n\n", atoi(operand));
			
			while(oi < LINE_SIZE && operand[oi] != '\0'){/*delete word*/
				operand[oi] = '\0';
			}
			oi = 0;
			continue;
		}
		
		else if(is_register(word)){/*Direct register addressing*/
			word[0] = word[1];
			word[1] = '\0';/*in order to stay only with the register number*/
			printf("The number of the register is: %d\n\n", atoi(word));
			continue;
		}
		
		else if(operand_is_label(word)){/*Direct addressing*/
			if(is_name_in_list(*head, word)){
				printf("Error! The label %s exists in the label list!\n\n", word);
			}
			printf("The label is: %s\n\n", word);
		}
		
		else{/*Accessing struct addressing*/
			strtok(word, ".");
			printf("The struct is: %s\n\n", word);
		}
		
		/**mi = *mi + 1;*/
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
    
    index = 0;

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
* 
* Note: the length of binary_string is always 10 because I only use this function  
* for switching a 10 bits in binary to 2 digits in base 32.
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
	
    for (i = 0; i < BINARY_LENGTH - 1; i += 5){/*BINARY_LENGTH - 1, because the actual length is 10, the 1 is for the null terminator*/
         /* Extract 5-digit binary segment */
        strncpy(binary_segment, binary_str + i, 5);
        binary_segment[5] = '\0'; 

        /* Convert binary segment to decimal */
        decimal = strtol(binary_segment, NULL, 2);

        /* Map decimal to corresponding base32 symbol */
        base32_representation[index] = base32_symbols[decimal];
        index++;
    }
    base32_representation[index] = '\0';
    
    return base32_representation;
}




/*Parameter:
* num: an integer number.
* 
* Returns: 
* num in binary.
*/
char* int_to_binary(int num){
	
	int i, bit;
	/*allocate memory for 8 bits and the null terminator*/
    char* binary = (char*)malloc(EIGHT_BITS * sizeof(char));
    if (binary == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
	
    /*start from the most significant bit*/
    for (i = 7; i >= 0; i--) {
        /*check if the bit at position i is set*/
        bit = (num >> i) & 1;
        binary[7 - i] = bit + '0'; /*convert integer to character '0' or '1'*/
    }
    binary[8] = '\0';
    return binary;
}


