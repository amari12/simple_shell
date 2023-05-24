#ifndef MAIN_H
#define MAIN_H

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 1024
#define ARGS_SIZE 50

extern char **environ;

/*file: process_input2.c*/
void get_input(char **input);
void get_input2(char **input);
void split_input(char *input, char *args[]);
void forking(char *args[], char *cmd, char *path, char *prog_name);
int check_spaces(char *input);

/*file: handle_bi.c*/
int handle_builtins(char *args[], char **environ, char *prog_name);
void change_dir(char *args[], char *prog_name);
void check_comments(char *input);

#endif
