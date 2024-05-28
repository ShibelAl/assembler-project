#define LABEL_LENGTH 31
#define OPCODE_LENGTH 4
#define LINE_SIZE 81 /*80 + the character '\n'*/
#define COMMAND_QTY 16 /*number of commands in the assembly language*/
#define REGISTER_QTY 8 /*number of registers in the given computer*/
#define MAX_DIGITS 4 /* Max length of base32 representation of the decimal number, 
3 characters for the number + 1 for null terminator */
#define BINARY_LENGTH 11 /*actual length is 10 bits + 1 for null terminator*/
#define MI_LENGTH 11 /*MI = MACHINE INSTRUCTION. actual length is 10 bits + 1 for null terminator*/
#define TWO_FIELDS 2 /*the two fields are memory address, and data/instruction. in the ram.*/
#define RAM_SIZE 256 /*according to the maman, the ram in the virtual computer has 256 cells*/
#define TEN_BITS 11/*it's 10 bits + 1 the null terminator*/
#define EIGHT_BITS 9/*it's 8 bits + 1 the null terminator*/
#define FOUR_BITS 5/*it's 4 bits + 1 the null terminator*/

enum boolean{FALSE, TRUE};

