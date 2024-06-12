void first_pass(FILE *input_fp);
void line_decode(char *line, int line_num, machine_code *machine_code_arr, int *mi, int *error, int *DC, int *IC);
void store_instruction_line(char *line, int i, machine_code *machine_code_arr, int *mi, char *command, labels **head, int *IC);
void store_instruction_line_operands(char *line, int i, machine_code *machine_code_arr, int *mi, labels **head, int *IC);

