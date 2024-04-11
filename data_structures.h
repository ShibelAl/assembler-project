#include "defines.h"


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



/*GOAL: when there is a label, this linked list stores the relevant information of it.
* 
* Parameters:
* name: name of the label.
* address: address of the label.
* is_external: TRUE if label is declared extern, FALSE otherwise.
* is_entry: TRUE if label is entry, FALSE otherwise.
* is_instruction: TRUE if current line is instruction line, FALSE if it's directive.
*/
typedef struct label_list{
  
  char *name; /*I need to allocate LABEL_LENGTH to each name*/
  int address; 
  int is_extern; 
  int is_entry; 
  int is_instruction; 
  struct label_list *next;     
  
}labels;

labels* create_label_node(const char *name, int address, int is_extern, int is_entry, int is_instruction);
void append_label_node(labels **head, labels **current, char *name, int address, int is_extern, int is_entry, int is_instruction);
void print_label_list(labels* head);
void free_label_list(labels* head);
int is_name_in_list(labels* head, char *name);



/*GOAL: storing registers names and their number in binary.
* NOTE: the binary expression consists of 4 elements even tough we have only 8 
* registers, and that's the project instructions.
*/
typedef struct registers{

 	char register_name[3];
	char num_of_register[5];
  
}registers;

extern registers registers_table[];















