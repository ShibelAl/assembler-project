#define OPCODE_LENGTH 31


/*#define LINE_SIZE 81
#define LABEL_LENGTH 31
#define OPCODE_LENGTH 4*/


/* For storing opcode specifications.
each opcode has a number from 1-15 to specify it from the others */
typedef struct opcode{
  
	char command[OPCODE_LENGTH]; /* what opcode (command) is it (e.g. mov, cmp, ...)*/
	char in_binary[5]; /* number of command (that's from 1-15 in decimal) in binary */
	int amount_of_operands; /* number of operands that command can use */
	int source_types[5]; /* legal source operand addressing types */
	int destination_types[5]; /* legal destination operand addressing types */
	
}opcode;

extern opcode opcodes_table[];


