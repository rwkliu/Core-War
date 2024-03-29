#ifndef HELPER_H
#define HELPER_H
#include "asm.h"

// function prototypes
int my_atoi(char *num);
int my_strlen(char* str);
char* my_strncpy(char* dst, char* src, int n);
char* my_strcpy(char* dst, char* src);
char* my_strcat(char *dst, char *src);
char *my_strchr(char* str_1, char char_1);
int my_strcmp(char* str_1, char* str_2);
int my_strncmp(char* str_1, char* str_2, int index);
void *my_memset(void *s, int c, size_t n);
void my_putstr(char *str);
void my_puterror(char *str);
// char *find_word(char* pointer, t_array* dictionary);

#endif