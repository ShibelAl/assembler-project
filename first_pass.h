void first_pass(FILE *input_fp);
int is_empty_line(char *line);
int is_label(char *word);
int is_alphabetic(char character);
int is_command(char *word);
char line_type(char *line, int i, int line_num, int *error);
void line_decode(char *line, int line_num, int *error, int *DC, int *IC);
