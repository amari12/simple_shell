#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * main - main function / entry point
 * Return: int
 */

int main(void)
{
	char *input;
	char *cmd;
	char *args[ARGS_SIZE];
	int exit_loop = 0, loops = 0, result;

	if (isatty(STDIN_FILENO) == 1)
	{ /*interactive mode*/
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
				result = forking(args, cmd); /*fork and exe child process*/
				loops++;
				if (result != 0)
				{
				/*	write_error(args, loops);*/
					args[0] = "exit";
					handle_builtins(args);
				}
				/*print stdout - stdout.txt*/
			}
			free(input);
		}
	}
	/*else non interactive mode*/
	return (0);
}

