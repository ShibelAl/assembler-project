#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "first_pass_utility_functions.h"



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
labels *first_pass(FILE *input_fp, machine_code *machine_code_arr, labels *head, labels *current){
	
	char *line;
	char *decimal_base32;
 	int IC, DC, line_num, error, mi, i, final_ic;
 	/*machine_code *machine_code_arr;*/
 	/*labels *head;
 	labels *current;
 	
 	head = NULL;
 	current = NULL;*/
 	
	IC = 100; /* Instruction Counter */
	DC = 0; /* Data Counter */
	line_num = 1;
	mi = 0; /*mi = machine_code_array index*/
	i = 0; /*index for printing the machine code in a loop*/
	error = FALSE;
	
	
	line = (char *)calloc(sizeof(char), LINE_SIZE);
	/*machine_code_arr = (machine_code *)calloc(MAX_DIGITS * TWO_FIELDS, RAM_SIZE);*//*each element in this array has two fields:
 	address, and the instruction/data*/
	if(line == NULL || machine_code_arr == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	fgets(line, LINE_SIZE, input_fp);
	
	while(!feof(input_fp)){
		
		if(!is_empty_line(line) && line[0] != ';'){/*if line is not empty nor it's a comment*/
			
			line_decode(line, line_num, machine_code_arr, &mi, &head, &current, &error, &DC, &IC);
			if(error == TRUE){
			/*printf("\nError!!\n");*/
				error = FALSE;
			}
			
		}
		
		fgets(line, LINE_SIZE, input_fp);
		line_num++;
	}
	
	return head;
	
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
void line_decode(char *line, int line_num, machine_code *machine_code_arr, int *mi, labels **head, labels **current, int *error, int *DC, int *IC){
	
	char type, word[LINE_SIZE];
	int i, wi; /*wi = word index*/
	int label_address;
	
 	i = 0;
	wi = 0;
	label_address = 0;
	
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
		
		if(is_label(word)){
		
			word[strlen(word) - 1] = '\0';/*removing the ':' from the label*/
			/*printf("the label is: %s\n", word);*/
			if(is_name_in_list(*head, word)){
				/*if the label is in the list, and it is not an entry on an extern, then it's a 
				regular label that entered the list more that once*/
				if(!is_entry_label(*head, word) && !is_extern_label(*head, word)){
					printf("Error in line %d - Multiple declarations of the same label.\n\n", line_num);
					*error = TRUE;				
				}
			}
			type = line_type(line, i, line_num, error);
			if(type == 'd'){/*if the current line is a directive statement.*/
				/*if the label does not exist in the label list, then append it to the label list*/
				if(is_entry_label(*head, word) == LABEL_NOT_EXIST && is_extern_label(*head, word) == LABEL_NOT_EXIST){
					label_address = base32_to_decimal(machine_code_arr[*mi - 1].address) + 1;
					append_label_node(head, current, word, label_address, FALSE, FALSE, FALSE);			
				}
				else if(is_entry_label(*head, word) == TRUE){
					label_address = base32_to_decimal(machine_code_arr[*mi - 1].address) + 1;
					update_label_fields(*head, word, label_address, FALSE, TRUE, FALSE);
				}
				else if(is_extern_label(*head, word) == TRUE){
					label_address = base32_to_decimal(machine_code_arr[*mi - 1].address);
					update_label_fields(*head, word, label_address, TRUE, FALSE, FALSE);
				}
			}
			else if(type == 'i'){/*if the current line is an instruction statement.*/
				/*if the label does not exist in the label list, then append it to the label list*/
				if(is_entry_label(*head, word) == LABEL_NOT_EXIST && is_extern_label(*head, word) == LABEL_NOT_EXIST){
					append_label_node(head, current, word, *IC, FALSE, FALSE, TRUE);			
				}
				else if(is_entry_label(*head, word) == TRUE){
					update_label_fields(*head, word, *IC, FALSE, TRUE, TRUE);
				}
				else if(is_extern_label(*head, word) == TRUE){
					update_label_fields(*head, word, *IC, TRUE, FALSE, TRUE);
				}
			}
			else if(type == 'x'){/*there is an error*/
				*error = TRUE;
				return;
			}
		
		}
		
		else if(is_command(word)){
			store_instruction_line(line, i, machine_code_arr, mi, word, head, IC);/*storing the address of the command,
			and the addressing methods code that is set according to each operand*/
			*mi = *mi + 1;
			store_instruction_line_operands(line, i, machine_code_arr, mi, head, IC);
			break;
		}
		
		else if(strcmp(word, ".data") == 0){
			label_address = base32_to_decimal(machine_code_arr[*mi-1].address) + 1;
			store_data_line(line, i, machine_code_arr, mi, &label_address);
		}
		
		else if(strcmp(word, ".string") == 0){
			label_address = base32_to_decimal(machine_code_arr[*mi-1].address) + 1;
			store_string_line(line, i, machine_code_arr, mi, &label_address);
		}
		
		else if(strcmp(word, ".struct") == 0){
			label_address = base32_to_decimal(machine_code_arr[*mi-1].address) + 1;
			store_struct_line(line, i, machine_code_arr, mi, &label_address);
		}
		
		/*if there is a declaration that the label is entry*/
		else if(strcmp(word, ".entry") == 0){
				
			while(line[i] == ' ' || line[i] == '\t'){/* skip spaces */
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
			
			/*check if the label have been declared as extern before, so if that happened,
			the assembler will not append the label to the label list*/
			if(is_extern_label(*head, word) == TRUE){
				printf("Error The label %s in line %d have been declared as extern previously.\n\n", word, line_num);
			}
			else{
				append_label_node(head, current, word, *DC, FALSE, TRUE, FALSE);			
			}
		}
		
		/*if there is a declaration that the label is extern*/
		else if(strcmp(word, ".extern") == 0){
			
			while(line[i] == ' ' || line[i] == '\t'){/* skip spaces */
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
			
			/*check if the label have been declared as entry before, so if that happened,
			the assembler will not append the label to the label list*/
			if(is_entry_label(*head, word) == TRUE){
				printf("Error The label %s in line %d have been declared as entry previously.\n\n", word, line_num);
			}
			else{
				append_label_node(head, current, word, *DC, TRUE, FALSE, FALSE);
			}
			/*printf("%s Is an extern label!!\n\n", word);*/
			
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
void store_instruction_line(char *line, int i, machine_code *machine_code_arr, int *mi, char *command, labels **head, int *IC){
	
	char word[LINE_SIZE];
	char opcode[11];
	char addressing_method[5];/*for storing addressing method binary codes*/
	char *decimal_base32, *binary_base32;
	int wi, oi;
	int ai;
	wi = 0; /*wi = word index*/
	oi = 0; /*oi = opcodes table index.*/
	ai = 0;
	strcpy(opcode, "");
	strcpy(addressing_method, "");
	
	machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	*(machine_code_arr[*mi].is_instruction) = TRUE;
	
	
	/*saving the command binary code*/
	for(; oi < COMMAND_QTY ; oi++){
		if(strcmp(command, opcodes_table[oi].command) == 0){
			
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);		
			free(decimal_base32);
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
		
		/*I arranged these if statements like that for a reason
		(for the similarity between the direct addressing and accessing struct addressing)*/
		if(word[0] == '#'){/*Immediate addressing*/
			strcat(addressing_method, "00");/*00 is the binary code for immediate addressing*/
		}
		
		else if(is_register(word)){/*Direct register addressing*/
			
			strcat(addressing_method, "11");/*11 is the binary code for register addressing*/
			
		}
		
		else if(operand_is_label(word)){/*Direct addressing*/
			strcat(addressing_method, "01");/*01 is the binary code for direct addressing*/
		}
		
		else{/*Accessing struct addressing*/
			strcat(addressing_method, "10");/*10 is the binary code for accessing struct addressing*/
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
	
	*IC = *IC + 1;
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
	char *token;
	int wi, oi;
	int is_first_operand;/*A flag to know if i'm now at the first operand in the command or the second one (if there is second..)
	and I made this flag to know how to encode the register, because it's different being the first operand or the second as a register*/
	wi = 0;/*wi = word index*/
	oi = 0;/*oi = operand index*/
	is_first_operand = TRUE;/*to differentiate between the first and second operand*/		
	
	while(line[i] != '\n' && line[i] != EOF){
		
		while(line[i] == ' ' || line[i] == '\t' || line[i] == ','){
			i++;
		}
		
		machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
		if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
			printf("\nmemory allocation failed\n");
			exit(1);
		}
		
		*(machine_code_arr[*mi].is_instruction) = TRUE;
		
		while(line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != ',' && line[i] != EOF){/*saving the operand in word*/
			word[wi] = line[i];
			wi++;
			i++;
		}
		word[wi] = '\0';
		wi = 0;
		
		if(word[0] == '#'){/*Immediate addressing*/
			is_first_operand = FALSE;/*I put this so if a register comes after this operand, I know how to 
			encode his data, because if a register comes as the first operand it is different than when it
			comes as the second operand, so it's a register thing..*/
			wi++;/*skip the #*/
			while(word[wi] != '\0'){/*putting the actual number in first operand*/
				operand[oi] = word[wi];
				wi++;
				oi++;
			}
			operand[oi] = '\0';
			oi = 0;
			
			/*saving the address*/
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);	
			free(decimal_base32);
			/*saving the data*/
			strcpy(binary, strcat(int_to_8_binary(atoi(operand)), "00"));/*atoi(operand) is the number after the #, e.g. #5 --> atoi(operand) = 5*/
			binary_base32 = binary_to_base32(binary);/*switching the binary expression to base 32*/
			strcpy(machine_code_arr[*mi].code, binary_base32);
			
			while(oi < LINE_SIZE && operand[oi] != '\0'){/*delete word*/
				operand[oi] = '\0';
			}
			oi = 0;
			*IC = *IC + 1;
		}
		
		
		else if(is_register(word)){/*Direct register addressing*/
			
			word[0] = word[1];
			word[1] = '\0';/*in order to stay only with the register number*/
			
			/*saving the address*/
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);	
			free(decimal_base32);
			
			/*if there is one register in the line*/
			if(number_of_registers(line) == 1){
				if(is_first_operand){
					/*look at the example table in the maman to understand why I did this*/
					strcpy(binary, int_to_4_binary(atoi(word)));
					strcat(binary, "0000");
					strcat(binary, "00");
					binary_base32 = binary_to_base32(binary);
					strcpy(machine_code_arr[*mi].code, binary_base32);
					*mi = *mi + 1;
					is_first_operand = FALSE;
				}
				/*if it's the second operand in the line*/
				else{
					strcpy(binary, "0000");
					strcat(binary, int_to_4_binary(atoi(word)));
					strcat(binary, "00");
					binary_base32 = binary_to_base32(binary);
					strcpy(machine_code_arr[*mi].code, binary_base32);
					*mi = *mi + 1;
				}
			}
			
			/*if there is two registers in the line*/
			else{
				if(is_first_operand){
					strcpy(binary, int_to_4_binary(atoi(word)));
					is_first_operand = FALSE;
				}
				/*if its the second operand in the line*/
				else{
					*IC = *IC - 1;/*because if there is two registers in the line, I don't want to increase the IC
					for each one of them. Look at the example table of the maman*/
					decimal_base32 = decimal_to_base32(*IC);
					strcpy(machine_code_arr[*mi].address, decimal_base32);	
					strcat(binary, int_to_4_binary(atoi(word)));
					strcat(binary, "00");
					binary_base32 = binary_to_base32(binary);
					strcpy(machine_code_arr[*mi].code, binary_base32);
					*mi = *mi + 1;
				}
			}
			*IC = *IC + 1;
			continue;
		}
		
		else if(operand_is_label(word)){/*Direct addressing*/
			machine_code_arr[*mi].label = (char *)malloc(LABEL_LENGTH);
			if(machine_code_arr[*mi].label == NULL){
				printf("\nmemory allocation failed\n");
				exit(1);
			}
			is_first_operand = FALSE;/*I put this so if a register comes after this operand, I know how to 
			encode his data, because if a register comes as the first operand it is different than when it
			comes as the second operand, so it's a register thing..*/
			/*saving the address*/
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);	
			free(decimal_base32);
			/*saving the data (the data of the label is it's address where it's declared)*/
			strcpy(machine_code_arr[*mi].code, word);
			strcpy(machine_code_arr[*mi].label, word);
			*IC = *IC + 1;
		}
		
		else{/*Accessing struct addressing*/
			is_first_operand = FALSE;/*I put this so if a register comes after this operand, I know how to 
			encode his data, because if a register comes as the first operand it is different than when it
			comes as the second operand, so it's a register thing..*/
			token = strtok(word, ".");
			
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);	
			free(decimal_base32);
			
			strcpy(machine_code_arr[*mi].code, word);
			
			*IC = *IC + 1;
			
			token = strtok(NULL, ".");/*now token has the struct's field number. (e.g. struct is S1.2 --> token = 2)*/
			
			*mi = *mi + 1;
			machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
			machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
			machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
			if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
				printf("\nmemory allocation failed\n");
				exit(1);
			}
			
			*(machine_code_arr[*mi].is_instruction) = TRUE;
			
			decimal_base32 = decimal_to_base32(*IC);
			strcpy(machine_code_arr[*mi].address, decimal_base32);
			free(decimal_base32);
			
			strcpy(binary, int_to_8_binary(atoi(token)));/*make the number of the struct be in 8 digits binary
														  (like in the example table in the maman)*/
			strcat(binary, "00");/*add 00 to the A.R.E field*/
			strcpy(machine_code_arr[*mi].code, binary_to_base32(binary)); /*convert the binary code into base 32*/
			
			*IC = *IC + 1;
		}
		
		*mi = *mi + 1;
	}
	
}


	







void store_data_line(char *line, int i, machine_code *machine_code_arr, int *mi, int *DC){
	
	char *delimiters = ", "; /* Delimiters: double quote, comma and space */
    char *token;
    int number;
    char *decimal_base32, *binary_base32;
    char data_expression[LINE_SIZE];
    int di; /*di = data_expression index*/
	
    di = 0;
    
    while(line[i] == ' ' || line[i] == '\t'){/*skip spaces*/
		i++;
	}
	
	while(line[i] != '\n'){
		data_expression[di] = line[i];
		i++;
		di++;
	}
	data_expression[di] = '\0';
	
    machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	*(machine_code_arr[*mi].is_instruction) = FALSE;
	
    /* Get the first token */
    token = strtok(data_expression, delimiters);
	
    /* Iterate over the rest of the tokens */
    while(token != NULL) {
    	
        number = string_to_int(token);
        decimal_base32 = decimal_to_base32(*DC);
		strcpy(machine_code_arr[*mi].address, decimal_base32);	
		/*converting the letter into binary, and then converting the binary expression to base32*/
		binary_base32 = binary_to_base32(int_to_10_binary(number));
		strcpy(machine_code_arr[*mi].code, binary_base32);
      
        /* Get the next token */
        token = strtok(NULL, delimiters);
       /* printf("token is: %s\n", token);*/
        *mi = *mi + 1;
        machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
		if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
			printf("\nmemory allocation failed\n");
			exit(1);
		}
		
		*(machine_code_arr[*mi].is_instruction) = FALSE;
		*DC = *DC + 1;
    }
		
		/*maybe I should free the allocated memory for the machine_code_arr..*/
		
}






void store_string_line(char *line, int i, machine_code *machine_code_arr, int *mi, int *DC){
	
	char *delimiters = "\", "; /* Delimiters: double quote, comma and space */
    char *token;
    char letter;
    char *decimal_base32, *binary_base32;
    char string_expression[LINE_SIZE];
    int si; /*si = string_expression index*/
    int ti; /*ti: token index*/
	
    si = 0;
    ti = 0;
    
    while(line[i] == ' ' || line[i] == '\t'){/*skip spaces*/
		i++;
	}
    
    while(line[i] != '\n'){
		string_expression[si] = line[i];
		i++;
		si++;
	}
    string_expression[si] = '\0';
    
    machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	*(machine_code_arr[*mi].is_instruction) = FALSE;
				 
    /* Get the first token */
    token = strtok(string_expression, delimiters);
	
    /* Iterate over the rest of the tokens */
    while(token[ti] != '\0') {
        /* each token is a single letter */
        letter = token[ti];
        ti++;
        decimal_base32 = decimal_to_base32(*DC);
		strcpy(machine_code_arr[*mi].address, decimal_base32);	
		/*converting the letter into binary, and then converting the binary expression to base32*/
		binary_base32 = binary_to_base32(int_to_10_binary((int)letter));
		strcpy(machine_code_arr[*mi].code, binary_base32);
		
        *mi = *mi + 1;
        machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
		if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
			printf("\nmemory allocation failed\n");
			exit(1);
		}
		
		*(machine_code_arr[*mi].is_instruction) = FALSE;
		*DC = *DC + 1;
    }
   
    	decimal_base32 = decimal_to_base32(*DC);
		strcpy(machine_code_arr[*mi].address, decimal_base32);	
		/*putting 0 in int_to_10_binary because of the null terminator - to be 0000000000*/
		/*printf("Letter: null\n");*/
		binary_base32 = binary_to_base32(int_to_10_binary(0));
		strcpy(machine_code_arr[*mi].code, binary_base32);
		
		*mi = *mi + 1;
		*DC = *DC + 1;
}















void store_struct_line(char *line, int i, machine_code *machine_code_arr, int *mi, int *DC){
	char *delimiters = "\", "; /* Delimiters: double quote, comma and space */
    char *token;
    char letter;
    int number;
    char *decimal_base32, *binary_base32;
    char string_expression[LINE_SIZE];
    int si; /*si = string_expression index*/
    int ti; /*ti: token index*/
	
    si = 0;
    ti = 0;
    
    while(line[i] == ' ' || line[i] == '\t'){/*skip spaces*/
		i++;
	}
    
    while(line[i] != '\n'){
		string_expression[si] = line[i];
		i++;
		si++;
	}
    
    machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	*(machine_code_arr[*mi].is_instruction) = FALSE;
	
	/*--- Here is the integer field ---*/
				 
    /* Get the first token */
    token = strtok(string_expression, delimiters);
    /*printf("the first token is: %s\n", token);*/
    
    number = string_to_int(token);
    decimal_base32 = decimal_to_base32(*DC);
	strcpy(machine_code_arr[*mi].address, decimal_base32);	
	/*converting the letter into binary, and then converting the binary expression to base32*/
	binary_base32 = binary_to_base32(int_to_10_binary(number));
	/*printf("the binary number after int_to_10_binary is: %s\n\n", int_to_10_binary(number));*/
	strcpy(machine_code_arr[*mi].code, binary_base32);
	    
    /* Get the next token */
    token = strtok(NULL, delimiters);
   /* printf("token is: %s\n", token);*/
    *mi = *mi + 1;
    machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
	machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
	if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	
	*(machine_code_arr[*mi].is_instruction) = FALSE;
	*DC = *DC + 1;
	
	/*--- Here is the string field ---*/
	
    /* Iterate over the rest of the tokens */
    while(token[ti] != '\0') {
        /* each token is a single letter */
        letter = token[ti];
        ti++;
        decimal_base32 = decimal_to_base32(*DC);
		strcpy(machine_code_arr[*mi].address, decimal_base32);	
		/*converting the letter into binary, and then converting the binary expression to base32*/
		binary_base32 = binary_to_base32(int_to_10_binary((int)letter));
		strcpy(machine_code_arr[*mi].code, binary_base32);
        /*here is my code*/
        
        /*printf("Letter: %c\n", letter);
		printf("%s %s		mi = %d, DC = %d\n\n", machine_code_arr[*mi].address, machine_code_arr[*mi].code, *mi, *DC);*/
		
        /* Get the next token */
        *mi = *mi + 1;
        machine_code_arr[*mi].address = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].code = (char *)calloc(sizeof(char), MAX_DIGITS);
		machine_code_arr[*mi].is_instruction = (int *)malloc(sizeof(int));
		if(machine_code_arr[*mi].address == NULL || machine_code_arr[*mi].code == NULL || machine_code_arr[*mi].is_instruction == NULL){
			printf("\nmemory allocation failed\n");
			exit(1);
		}
		
		*(machine_code_arr[*mi].is_instruction) = FALSE;
		*DC = *DC + 1;
    }
   
    	decimal_base32 = decimal_to_base32(*DC);
		strcpy(machine_code_arr[*mi].address, decimal_base32);	
		/*putting 0 in int_to_10_binary because of the null terminator - to be 0000000000*/
		/*printf("Letter: null\n");*/
		binary_base32 = binary_to_base32(int_to_10_binary(0));
		strcpy(machine_code_arr[*mi].code, binary_base32);
		
		/*printf("%s %s		mi = %d, DC = %d\n\n", machine_code_arr[*mi].address, machine_code_arr[*mi].code, *mi, *DC);*/
		
		*mi = *mi + 1;
		*DC = *DC + 1;
}














































