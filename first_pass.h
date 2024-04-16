void first_pass(FILE *input_fp);
int is_empty_line(char *line);
int is_label(char *word);
int is_alphabetic(char character);
int is_command(char *word);
char line_type(char *line, int i, int line_num, int *error);
void line_decode(char *line, int line_num, machine_code *machine_code_arr, int mi, int *error, int *DC, int *IC);
void store_instruction_line(char *line, int i, machine_code *machine_code_arr, int mi, char *command, int *IC);
int is_register(char *word);
int operand_is_label(char *word);
void store_opcode(char *line); /*currently not used*/
char *decimal_to_base32(int decimal);
char *binary_to_base32(char *binary_str);

