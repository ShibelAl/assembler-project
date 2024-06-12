int is_empty_line(char *line);
int is_label(char *word);
int is_alphabetic(char character);
int is_command(char *word);
char line_type(char *line, int i, int line_num, int *error);
int is_register(char *word);
int operand_is_label(char *word);
char *decimal_to_base32(int decimal);
char *binary_to_base32(char *binary_str);
char* int_to_10_binary(int num);
char* int_to_8_binary(int num);
char* int_to_4_binary(int num);
int number_of_registers(char *line);
