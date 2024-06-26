#include "../include/asm_parse.h"
#include "../include/helper.h"
#include "../include/memory.h"
#include "../include/tokenize.h"

int execute_asm(char *filename) {
    FILE *cor = create_cor_file(filename);
    t_header *header = init_header();
    t_node *inst_head = NULL;
    t_prog_size size = {
        .num_inst = 0,
        .total_bytes = 0,
        .curr_byte = 0
    };

    const char *read = "r";
    FILE *fp = fopen(filename, read);
    if (!fp) return EXIT_FAILURE;
    inst_head = read_file(fp, &header, &size);
    header->prog_size = (size.total_bytes << 24) | size.num_inst;
    fclose(fp);

    if (write_header(cor, header)) return EXIT_FAILURE;
    if (write_inst(cor, inst_head, &size)) return EXIT_FAILURE;

    free_nodes(inst_head);
    free(header);
    fclose(cor);

    return EXIT_SUCCESS;
}

t_node *read_file(FILE *fp, t_header **header, t_prog_size *size) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    t_node *head = NULL;

    while ((nread = getline(&line, &len, fp)) != -1) {
        if (my_strncmp(line, NAME_CMD_STRING, 5) == 0) { 
            remove_line_title((*header)->prog_name, line, 7);
        }
        else if (my_strncmp(line, COMMENT_CMD_STRING, 8) == 0) { 
            remove_line_title((*header)->comment, line, 10);
        } 
        else if (nread == 1 || my_strncmp(line, "/n", 1) == 0) {
            continue;
        } 
        else {
            size->num_inst++;
            if (line[nread - 1] == '\n') line[nread - 1] = '\0';

            if (!head) {
                head = string_to_node(line, size);
                head->id = size->num_inst;
            } else {
                t_node *tmp = head;
                while (tmp->next) tmp = tmp->next;
                tmp->next = string_to_node(line, size);
                tmp->next->id = size->num_inst;

                // calculate offset
                if (tmp->next->id == 2) {
                    tmp->next->offset += tmp->num_bytes;
                } else {
                    tmp->next->offset += tmp->num_bytes + tmp->offset;
                }
            }
        }
    }
    
    print_nodes(head);  // TESTING
    return head;
}

FILE *create_cor_file(char *filename) {
    char *name = replace_ext(filename);
    const char *write = "wb";
    FILE *cor= fopen(name, write);
    return cor;
}

char *replace_ext(char *filename) {
    int len = my_strlen(filename);
    char *new_ext = "cor";
    char *new_filename = init_str(len + 2);
    my_strncpy(new_filename, filename, len);
    my_strcat(new_filename, new_ext);

    return new_filename;
}

void remove_line_title(char *dest, char *line, int size) {
    my_strcpy(dest, line + size); 
    int copied_len = my_strlen(dest);
    dest[copied_len - 2] = '\0';
    dest[copied_len - 1] = '\0';
}

t_node *string_to_node(char *src, t_prog_size *size) {
    t_array *tokens = tokenizer(src, SEPARATOR_SET);

    t_node *args = init_node(4);
    int i = 0;
    
    int tok_len = my_strlen(tokens->array[0]);
    if (tokens->array[0][tok_len - 1] == LABEL_CHAR) {
        args->label = tokens->array[0];
        args->label[tok_len - 1] = '\0';
        i++;
    }

    tok_len = my_strlen(tokens->array[i]);
    args->command = init_str(tok_len + 1);
    my_strcpy(args->command, tokens->array[i]);
    i++;

    if (is_special_command(args->command)) args->num_bytes -= 1;

    while (tokens->array[i]) {   
        if (tokens->array[i][0] == COMMENT_CHAR) continue;
        else if (tokens->array[i][0] == 'r') {
            ttoa_remove_char(&args, tokens->array[i], 1);
            args->num_bytes++;
            args->param_count++;
        }
        else if (tokens->array[i][0] == DIRECT_CHAR) {
            ttoa_remove_char(&args, tokens->array[i], 2);
            args->num_bytes += 4;
            args->param_count++;
        }
        else {
            token_to_arg(&args, tokens->array[i], 3);
            args->num_bytes += 2;
            args->param_count++;
        }
        i++;
    }
    
    size->total_bytes += (args->num_bytes);
    free_t_array(tokens);

    return args;
}

void token_to_arg(t_node **args, char *tok, int type) 
{
    (*args)->array[(*args)->param_count] = init_arg();
    t_arg *tmp = (*args)->array[(*args)->param_count];
    tmp->arg = init_str(my_strlen(tok) + 1);
    my_strcpy(tmp->arg, tok);
    tmp->type = type;
}

void ttoa_remove_char(t_node **args, char *tok, int type) 
{
    (*args)->array[(*args)->param_count] = init_arg();
    t_arg *tmp = (*args)->array[(*args)->param_count];
    tmp->arg = init_str(my_strlen(tok));
    my_strcpy(tmp->arg, tok + 1);
    tmp->type = type;
}

bool is_special_command(char *command) {
    if (my_strcmp(command, "live") == 0) return true;
    if (my_strcmp(command, "zjmp") == 0) return true;
    if (my_strcmp(command, "fork") == 0) return true;
    if (my_strcmp(command, "lfork") == 0) return true;
    if (my_strcmp(command, "aff") == 0) return true;

    return false;
}
