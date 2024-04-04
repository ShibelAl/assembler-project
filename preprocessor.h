typedef struct macro{
     
	char *macro_name;
	char *macro_content;
     
}macro; 


FILE* store_macro(FILE *input_fp, FILE *output_fp);
int is_macro(char *line);
int is_macro_name(char *line, macro *macro_arr);
void put_macro_content(char *line, macro *macro_arr, FILE *output_fp);
