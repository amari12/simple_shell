#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * main - main funtion / entry poin
 * @argc: nr of arguments
 * @argv: arguments
 * Return: int
 */

int main(int argc __attribute__((unused)),
		char *argv[] __attribute__((unused)))
{
	char *input;
	char *cmd;
	char *args[ARGS_SIZE];
	int exit_loop = 0, loops = 0; /*, result;*/
	int og_stdout_fd, c, stdout_fd;
	FILE *stdout_file;

	if (isatty(STDIN_FILENO) == 1)
	{ /*interactive mode*/
		og_stdout_fd = dup(STDOUT_FILENO);
		while (exit_loop != 1) /*shell loop*/
		{
			write(STDOUT_FILENO, "our shell>> ", 12);
			fflush(stdout);
			input = get_input(); /*read input*/
			/*get_input2(&input);*/ /*advanced*/
			if (input != NULL)
			{
				check_comments(input); /*search & handle comments*/
				split_input(input, args); /*separate input -> args list*/
				cmd = args[0];
				if (handle_builtins(args) == 1) /*handle builtins*/
					continue; /*env builtin or cd -> restart loop*/
			/*	result =*/
				forking(args, cmd); /*fork and exe child process*/
				loops++;
				/*if (result != 0)*/
				/*{*/
				/*	write_error(args, loops);*/
				/*}*/
				/*print stdout - stdout.txt*/
				stdout_fd = open("stdout.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(stdout_fd, STDOUT_FILENO);
				close(stdout_fd);
				stdout_file = fopen("stdout.txt", "r");
				if (stdout_file != NULL)
				{
					while ((c = fgetc(stdout_file)) != EOF)
					{
						putchar(c);
					}
					fclose(stdout_file);
				}
				dup2(og_stdout_fd, STDOUT_FILENO);
			}
			free(input);
		}
		close(og_stdout_fd);
	}
	/*else non interactive mode*/
	return (0);
}

