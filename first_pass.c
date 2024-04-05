#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

void first_pass(FILE *input_fp);
int empty_line(char *line);

int main(){
	
	FILE *input_file;
    input_file = fopen("test.as", "r");  

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
 	int IC, DC, i, line_num;
  
	IC = 0;
	DC = 0;
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
		
		if(empty_line(line) || line[0] == ';'){/*if line is empty or it's a comment*/
			line_num++;
			fgets(line, LINE_SIZE, input_fp);
		}
		
		else/* if()*/{
			printf("Hello World!\n");
		}
		
		fgets(line, LINE_SIZE, input_fp);
	}
	
}




/*returns true if parameter line has only blanks*/
int empty_line(char *line){
	
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





