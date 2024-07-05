#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "first_pass_utility_functions.h"

void second_pass(FILE *input_fp, labels *head, machine_code *machine_code_arr){
	char *line;
	int i, DC, final_ic;
	char *decimal_base32;
	i = 0;
	
	/*itirate over the machine code arr*/
	while(machine_code_arr[i].address != NULL){
		/* if the string in machine_code_arr[i].code is a label (in lables list) */
		if(is_name_in_list(head, machine_code_arr[i].code)){
			/* if the label is extern */
			if(is_extern_label(head, machine_code_arr[i].code) == TRUE){
				/* the binary code for external label is 0000000001, so it's 1  (look at example table in maman) */
				strcpy(machine_code_arr[i].code, decimal_to_base32(1));
			}
			else{
				/* the binary code for a non-external label ends with 10, so the first 8 bits that represents the address of 
				the label gets rotated to left two times, which multiplys the number by 2^2 (4) and then comes the 10 at the left,
				which adds 2 to the number, so the total is: label address * 4 + 2. */
				strcpy(machine_code_arr[i].code, decimal_to_base32(get_label_address(head, machine_code_arr[i].code) * 4 + 2));
			}
		}
		i++;
	}
	i = 0;
	
	
	line = (char *)calloc(sizeof(char), LINE_SIZE);
	if(line == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	printf("\n\n ----- now it's the second pass -----\n\n\n");
	
	printf("Label List:\n");
	print_label_list(head);
	printf("\n\n\n\n\n");
	
	/* itirates over the machine code array to reach the final ic */
	while(machine_code_arr[i].address != NULL){
		if(*machine_code_arr[i].is_instruction == TRUE){
			final_ic = base32_to_decimal(machine_code_arr[i].address);
		}
		i++;
	}
	
	i = 0;
	DC = final_ic + 1;
	/* Updating the DC. For each element in the array that is data element (not in instruction line)
	put the new DC, that starts from final_ic, to make the data after the instructions. */
	while(machine_code_arr[i].address != NULL){
		if(*machine_code_arr[i].is_instruction == FALSE){
			decimal_base32 = decimal_to_base32(DC);
			strcpy(machine_code_arr[i].address, decimal_base32);
			DC++;
		}
		i++;
	}
	free(decimal_base32);
	
	printf("\nmachine code:\n\n");
	i = 0;
	/* print all the instruction elements */
	while(machine_code_arr[i].address != NULL){
		if(*machine_code_arr[i].is_instruction == TRUE){
			printf("%s %s\n\n", machine_code_arr[i].address, machine_code_arr[i].code);
		}
		i++;
	}
	
	i = 0;
	/* now print the data elements */
	while(machine_code_arr[i].address != NULL){
		if(*machine_code_arr[i].is_instruction == FALSE){
			printf("%s %s\n\n", machine_code_arr[i].address, machine_code_arr[i].code);
		}
		i++;
	}
	
	
	/*fgets(line, LINE_SIZE, input_fp);
	while(!feof(input_fp)){
		printf("%s\n", line);
		fgets(line, LINE_SIZE, input_fp);
	}*/
}

