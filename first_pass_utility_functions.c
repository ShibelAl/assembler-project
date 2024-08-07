#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> /*for isdigit() in char_to_int(...)*/
#include "data_structures.h"
#include "first_pass_utility_functions.h"

/*-----------------------## This file contains all the utility functions of first_pass.c file ##---------------------------*/


/*Parameter: 
* line: the current line in the file
* 
* Returns:
* True, if the line is empty, or just blanks, and False, if not.
*/
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




/*Parameter: 
* word: a string that is one word, without blanks.
* 
* Returns:
* True, if the word is a label - like LOOP:, or MAIN: - (with the ':') and False otherwise.
*/
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




/*Parameter: 
* character: a character.
* 
* Returns:
* True, if the character is alphabetic, and False otherwise.
*/
/*returns true if parameter character is an alphabet letter, false otherwise*/
int is_alphabetic(char character){
    
	if((character >= 65 && character <= 90) || (character >= 97 && character <= 122))
		return TRUE;
  
	else return FALSE;
}




/*Parameter: 
* word: a string that is one word, without blanks.
* 
* Returns:
* True if parameter word is a command from the 16 commands of the language, False otherwise.
*/
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





/*Parameter: 
* word: a string that is one word, without blanks.
* 
* Returns:
* True if the parameter word is a name of a register, False otherwise.
*/
int is_register(char *word){
	
	int i;
	
	for(i = 0 ; i < REGISTER_QTY ; i++){
		if(strcmp(word, registers_table[i].register_name) == 0){
			return TRUE;
		}
	}
	return FALSE;
}





/*Parameter: 
* word: a string that is one word, without blanks.
* 
* Returns:
* True, if the parameter word is a label as an operand in an instruction statement (without the ':'), False otherwise.
*/
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
* decimal: an integer number.
* 
* Returns:
* The number decimal in base 32.
*/
char *decimal_to_base32(int decimal){
    int index, i, j; /* i, j is for reversing a string */
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

    /* Add a leading zero if the length is less than 2 */
    if (index < 2) {
        base32_representation[index] = base32_symbols[0];
        index++;
    }

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
* The integer num in 10 bit binary expression.
*/
char* int_to_10_binary(int num){
	
	int i, bit;
	/*allocate memory for 8 bits and the null terminator*/
    char* binary = (char*)malloc(TEN_BITS * sizeof(char));
    if (binary == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
	
    /*start from the most significant bit*/
    for (i = 9; i >= 0; i--) {
        /*check if the bit at position i is set*/
        bit = (num >> i) & 1;
        binary[9 - i] = bit + '0'; /*convert integer to character '0' or '1'*/
    }
    binary[10] = '\0';
    return binary;
}






/*Parameter:
* num: an integer number.
* 
* Returns: 
* The integer num in 8 bit binary expression.
*/
char* int_to_8_binary(int num){
	
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




/*Parameter:
* num: an integer number.
* 
* Returns: 
* The integer num in 4 bit binary expression.
*/
char* int_to_4_binary(int num){
	
	int i, bit;
	/*allocate memory for 8 bits and the null terminator*/
    char* binary = (char*)malloc(FOUR_BITS * sizeof(char));
    if (binary == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
	
    /*start from the most significant bit*/
    for (i = 3; i >= 0; i--) {
        /*check if the bit at position i is set*/
        bit = (num >> i) & 1;
        binary[3 - i] = bit + '0'; /*convert integer to character '0' or '1'*/
    }
    binary[4] = '\0';
    return binary;
}






/*Parameter: 
* line: the current line in the document.
*
* Returns: how many registers in line (could be 0, 1 or 2).
*/
int number_of_registers(char *line){
	int i, count, wi;
	char word[LINE_SIZE];
	i = 0;
	count = 0;/*number of the registers in the command (it could be 1 or 2)*/	
	wi = 0;
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
		
		if(is_register(word)){
			count++;
		}
	}
	return count;
}








/*Parameters:
* str - a string that represents a number (positive or negative)
* 
* Returns:
* an int number that is the number in the string form.
* for example if str = "-9" then it returns -9 as an int number.
*/
int string_to_int(char* str){
    int result = 0;  /* To store the final integer value */
    int sign = 1;    /* To handle the sign of the number */
    int i = 0;       /* Index for iterating through the string */

    /* Check if the first character is a negative sign */
    if (str[0] == '-') {
        sign = -1;  /* Set the sign to negative */
        i = 1;      /* Start from the next character */
    }

    /* Loop through the rest of the string */
    for (; str[i] != '\0'; i++){
        if (!isdigit(str[i])){
            printf("Invalid character in input string: %c\n", str[i]);
            return 0;  /* Return 0 for invalid input */
        }
        result = result * 10 + (str[i] - '0');  /* Convert char to int and add to result */
    }

    return sign * result;  /* Apply the sign to the result */
}







/*Parameters: 
* base32: a string representing a number in base 32.
* 
* Returns:
* The decimal representation of the base 32 number.
* 
* This function converts a base 32 encoded string to a decimal integer.
*/
int base32_to_decimal(char *base32){
    int decimal = 0; /* Initialize the result variable to store the decimal value */
    int length = strlen(base32); /* Get the length of the base32 input string */
    char base32_symbols[] = "!@#$%^&*<>abcdefghijklmnopqrstuv";
    int i, j, value;

    for (i = 0; i < length; i++) {
        value = -1; /* Initialize value to -1 to detect invalid characters */
        /* Find the value of the current base32 character */
        for (j = 0; j < 32; j++) {
            if (base32[i] == base32_symbols[j]) {
                value = j; /* Assign the index of the symbol to value */
                break; /* Exit the inner loop once the character is found */
            }
        }
        if (value == -1) { /* Check if the character was not found in base32_symbols */
            printf("Invalid character found in base32 input.\n");
            exit(1);
        }
        
        /* Update the decimal value by shifting the previous value left by 32 (base 32) and adding the current digit's value */
        decimal = decimal * 32 + value;
    }

    return decimal;
}







/*Parameters:
* machine_code_arr: the array which stores the machine code and their addresses
* according to the IC and DC.
* 
* final_ic: is the final ic that the program reached
* 
* This function updates the DC to start after the IC finished, it builds a new machine code array 
* and itirates over the original machine_code_arr.
* 
*/
/*void update_dc(machine_code *machine_code_arr, int final_ic){
	int dc;
	int i;
	int length = sizeof(machine_code_arr) / sizeof(machine_code_arr[0]);
	dc = ic + 1;
	
	for(i = 0; i < length ; i++){
		if(machine_code_arr[i].is_instruction == FALSE){
			
		}
	}
}


when printing the */







