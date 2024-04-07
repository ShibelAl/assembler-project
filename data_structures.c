#include <stdlib.h>
#include "defines"
#include "data_structures.h"


opcode opcodes_table[] = {

	{"mov", "0000", 2, {0,1,2,3,-1}, {1,2,3,-1}},
	{"cmp", "0001", 2, {0,1,2,3,-1}, {0,1,2,3,-1}},
	{"add", "0010", 2, {0,1,2,3,-1}, {1,2,3,-1}},
	{"sub", "0011", 2, {0,1,2,3,-1}, {1,2,3,-1}},
	{"not", "0100", 1, {-1}, {1,2,3,-1}},
	{"clr", "0101", 1, {-1}, {1,2,3,-1}},
	{"lea", "0110", 2, {1,2,-1}, {1,2,3,-1}},
	{"inc", "0111", 1, {-1}, {1,2,3,-1}},
	{"dec", "1000", 1, {-1}, {1,2,3,-1}},
	{"jmp", "1001", 1, {-1}, {1,2,3,-1}},
	{"bne", "1010", 1, {-1}, {1,2,3,-1}},
	{"get", "1011", 1, {-1}, {1,2,3,-1}},
	{"prn", "1100", 1, {-1}, {0,1,2,3,-1}},
	{"jsr", "1101", 1, {-1}, {1,2,3,-1}},
	{"rts", "1110", 0, {-1}, {-1}},
	{"hlt", "1111", 0, {-1}, {-1}}

};