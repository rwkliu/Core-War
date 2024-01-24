#include "../include/op.h"
#include "../include/champion.h"
#include <sys/fcntl.h>

// initialize champion
champion_t *init_champion(void) {
    champion_t *new_champ = malloc(sizeof(champion_t));
    if (new_champ == NULL) {
        return NULL;
    }
    new_champ->champ_header = NULL;
    new_champ->id = 0;
    new_champ->address = 0;
    new_champ->instructions = NULL;
    new_champ->next = NULL;

    new_champ->free_champion = free_champion;
    new_champ->load_champion = load_champion;
    return new_champ;
}

int read_champion(champion_t *champ, char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }

    champ->champ_header = get_header(filename, fd); 
    champ->instructions = get_instructions(filename, fd);
}

// create champion header
header_t *get_header(char *filename, int fd) {
    header_t *new_header = malloc(sizeof(header_t));
    if (new_header == NULL) {
        return NULL;
    }

    // TODO read header from file

    return new_header;
}

// create champion instructions
op_t *get_instructions(char *filename, int fd) {
    op_t *new_instructions = malloc(sizeof(op_t));
    if (new_instructions == NULL) {
        return NULL;
    }

    // TODO read instructions from file

    return new_instructions;
}


// store champion in core