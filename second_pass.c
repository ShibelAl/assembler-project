#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"

void second_pass(FILE *input_fp, labels *head){
	char *line;
	
	line = (char *)calloc(sizeof(char), LINE_SIZE);
	if(line == NULL){
		printf("\nmemory allocation failed\n");
		exit(1);
	}
	printf("\n\n ----- now it's the second pass -----\n\n\n");
	
	printf("Label List:\n");
	print_label_list(head);
	printf("\n\n\n\n\n");
		
	
	/*fgets(line, LINE_SIZE, input_fp);
	while(!feof(input_fp)){
		printf("%s\n", line);
		fgets(line, LINE_SIZE, input_fp);
	}*/
}

