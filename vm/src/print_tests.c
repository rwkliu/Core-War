#include "../include/print_tests.h"


// Print the contents of the 16 general purpose, ac, and carry flag registers
void print_header(header_t *header) {
	printf("-----Printing champion \"%s\" header contents-----\n", header->prog_name);
	printf("magic: %i\n", header->magic);
    printf("prog_size: %i\n", header->prog_size);
	printf("comment: %s\n", header->comment);
}

// Print the inst buf hex by hex
void print_inst_buf(header_t *header, u_int8_t *buf, size_t size) {
	printf("-----Printing champion \"%s\" instruction raw data-----\n", header->prog_name);

    // Print the contents of the buffer
    for(int i = 0; i < size; i++) {
        printf("buf[%i] = %02x\n", i, buf[i]);
    }
}

// Print the contents of the 16 general purpose, ac, and carry flag registers
void print_cursor(cursor_t *cursor) {
	printf("-----Printing champion %d register contents-----\n", cursor->id);
	for (int i = 0; i < REG_NUMBER; i++) {
		printf("register %d: %d\n", i + 1, cursor->reg[i]);
	}
	printf("ac: %d\n", cursor->ac);
	printf("carry: %d\n", cursor->carry);
}

// print champion
void print_champions(champion_t *head) {
    champion_t *curr = head;
    while (curr) {
        // print
        printf("\nChamp is named %s\n", curr->name);
        curr = curr->next;
    }
}