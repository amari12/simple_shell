#ifndef MAIN_H
#define MAIN_H

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE 1024
#define ARGS_SIZE 50

extern char **environ;

/*file: process_input2.c*/
char *get_input();
void get_input2(char **input);
void split_input(char *input, char *args[]);
int forking(char *args[], char *cmd);
int check_spaces(char *input);

void write_error(char *args[], int loops);
char *check_path(char *args[]);

/*file: handle_bi.c*/
int handle_builtins(char *args[]);
void change_dir(char *args[]);
void check_comments(char *input);

/*file: own_ functions.c*/
void itoa(int nr, char *str);

#endif
