#ifndef _ASM_H_
#define _ASM_H_
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>


#define MEM_SIZE (6 * 1024)
#define IDX_MOD 512     // modulo of the index
#define CHAMP_MAX_SIZE (MEM_SIZE / 10)
#define MAX_INSTRUCTIONS 40
// TODO add CHAMP_MAX_SIZE check and remove MAX_INSTRUCTIONS

#define PROG_NAME_LENGTH 128
#define COMMENT_LENGTH 2048
#define COREWAR_EXEC_MAGIC 0xea83f3


#define COMMENT_CHAR '#'
#define LABEL_CHAR ':'
#define DIRECT_CHAR '%'
#define SEPARATOR_CHAR ','
#define SEPARATOR_SET ", \t"

#define LABEL_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"

/* size (in bytes) */
#define IND_SIZE 2
#define DIR_SIZE 4

#define NAME_CMD_STRING ".name"
#define COMMENT_CMD_STRING ".comment"

// define commands
#define live 0x01
#define ld 0x02
#define st 0x03 
#define add 0x04
#define sub 0x05
#define and 0x06
#define or 0x07
#define xor 0x08
#define zjmp 0x09
#define ldi 0x0a
#define sti 0x0b
#define fork 0x0c
#define lld 0x0d
#define lldi 0x0e
#define lfork 0x0f
#define aff 0x10

// structs
typedef struct s_header {
  int magic;
  char prog_name[PROG_NAME_LENGTH + 1];
  int prog_size;
  char comment[COMMENT_LENGTH + 1];
} t_header;

typedef struct s_prog_size {
    int curr_byte;
    int num_inst;
    int total_bytes;
} t_prog_size;

typedef struct s_string_array {
    int size;
    char **array;
} t_array;

typedef struct s_arg {
    u_int8_t type;
    char *arg;
} t_arg;

typedef struct s_arg_node {
    int id;
    int param_count;
    int num_bytes;
    int offset;
    char *label;
    char *command;
    t_arg **array;
    struct s_arg_node *next;
} t_node;

typedef struct {
    char token[25];
    u_int8_t command;
} command_map;

// function prototypes
int write_header(FILE *cor, t_header *header);
int write_inst(FILE *cor, t_node *head, t_prog_size *size);
u_int8_t get_command(FILE *cor, char *command);
u_int8_t *get_values(t_node *head, t_node *inst, t_prog_size *size);
void write_int_big_end(FILE *cor, int num);
u_int32_t calculate_jump(t_node *head, t_node *inst, char *label, t_prog_size *size);
void print_usage();

#endif