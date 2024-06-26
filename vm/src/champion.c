#include "../include/op.h"
#include "../include/helper.h"
#include "../include/champion.h"
#include "../include/vm_parse.h"
#include "../include/memory.h"
#include "../include/print_tests.h"
#include <sys/fcntl.h>


champion_t *create_champ(flag_t *flags, char *filename) {
    int fd = open(filename, O_RDONLY, 0644);
    if (!fd) {
        return NULL;
    }

    champion_t *champ = init_champion(flags);

    read_file(&champ, fd);

    close(fd);

    return champ;
}

champion_t *init_champion(flag_t *flags)  {
    champion_t *champ = malloc(sizeof(champion_t));
    if (champ == NULL) { return NULL; }

    champ->name = NULL;
    champ->comment = NULL;
    champ->string = NULL;
    champ->string_len = 0;

    if (flags->id) champ->id = flags->id;  
    else champ->id = flags->num_champions + 1;

    champ->time_of_death = 0;
    champ->cursor = init_cursor(flags);
    champ->cursor->parent = champ;
    champ->next = NULL;

    return champ;
}

cursor_t *init_cursor(flag_t *flags) {
    cursor_t *cursor = malloc(sizeof(cursor_t));
    if (cursor == NULL) { return NULL; }

    cursor->next = NULL;
    cursor->parent = NULL;
    cursor->dead = false;
    cursor->live = false;
    cursor->carry = 0;

    if (flags->address) cursor->index_start = flags->address;
    else cursor->index_start = 0;

    cursor->ac = cursor->index_start;
    cursor->num_inst = 0;
    cursor->current_inst = 0;
    cursor->opcode = -1;
    cursor->running = 0;
    cursor->cycle = 0;

    for (int i = 0; i < 16; i++) {
        cursor->reg[i] = 0;
    }

    return cursor;
}

// parse champion data
int read_file(champion_t **champ, int fd) {
    // header data
    ssize_t bytes;
    header_t *header = malloc(sizeof(header_t));
    bytes = read(fd, header, sizeof(header_t)); 

    /* TODO: add MAGIC check */ 

    (*champ)->name = init_and_strncpy(header->prog_name);
    (*champ)->comment = init_and_strncpy(header->comment);
    (*champ)->string_len = (header->prog_size >> 24); 
    (*champ)->cursor->num_inst = MASK_FF(header->prog_size);

    // instruction data
    char buf[(*champ)->string_len];
    memset(buf, 0, (*champ)->string_len);
    bytes = read(fd, &buf, sizeof(char) * (*champ)->string_len);
    (*champ)->string = buf;

    free(header);

    return EXIT_SUCCESS;
}
