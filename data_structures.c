#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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






/*------------------ Implementing label_list ------------------*/

/* Create a new node with a given name */
labels* create_label_node(const char *name, int address, int is_extern, int is_entry, int is_instruction) {
    labels* new_node = (labels*)malloc(sizeof(labels));
    if(new_node == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    new_node -> name = (char*)malloc(LABEL_LENGTH);
    strcpy(new_node -> name, name);
    if(new_node-> name == NULL){
        printf("Memory allocation failed\n");
        free(new_node); /*free the allocated node memory if strdup fails*/
        exit(1);
    }
    new_node -> address = address;
    new_node -> is_extern = is_extern;
    new_node -> is_entry = is_entry;
    new_node -> is_instruction = is_instruction;
    new_node -> next = NULL;
    return new_node;
}


/* Appends a new label node to the end of the linked list */
void append_label_node(labels **head, labels **current, char *name, int address, int is_extern, int is_entry, int is_instruction){
    if (*head == NULL) {
        /* if the list is empty, create the first node */
        *head = create_label_node(name, address, is_extern, is_entry, is_instruction);
        *current = *head;
    } 
    
    else {
        /* otherwise, append a new node to the end */
        (*current) -> next = create_label_node(name, address, is_extern, is_entry, is_instruction);
        *current = (*current) -> next;
    }
}


/* Function to print all the labels in the linked list */
void print_label_list(labels *head){
    while(head != NULL){
        printf("Name: %s, Address: %d, is_extern: %d, is_entry: %d, is_instruction: %d\n",
        	head -> name, head -> address, head -> is_extern, head -> is_entry, head -> is_instruction);
        head = head -> next;
	}
}


/* Function to free the memory allocated for the linked list */
void free_label_list(labels *head){
    labels* temp;
    while(head != NULL){
        temp = head;
        head = head -> next;
        free(temp -> name); /* Free the memory allocated for the name */
        free(temp);
    }
}



/* Check if a name already exists in the linked list */
int is_name_in_list(labels *head, char *name){
    while(head != NULL){
        if(strcmp(head -> name, name) == 0){
            return TRUE;
        }
        head = head -> next;
    }
    return FALSE;
}












/*------------------ Implementing register_table ------------------*/

registers registers_table[] = {

	{"r0", "0000"},
	{"r1", "0001"},
	{"r2", "0010"},
	{"r3", "0011"},
	{"r4", "0100"},
	{"r5", "0101"},
	{"r6", "0110"},
	{"r7", "0111"} 
     
};





















/*int main(){
	
	

    return 0;
}*/

