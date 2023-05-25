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
	/*char *exit_msg = "EOF\n...\n";*/
	char *cmd;
	char *args[ARGS_SIZE];
	int exit_loop = 0, loops = 0, result;

	if (isatty(STDIN_FILENO) == 1)
	{ /*interactive mode*/
		/*path = getenv("PATH");*/ /*get path*/
		while (exit_loop != 1) /*shell loop*/
		{
			write(STDOUT_FILENO, "our shell>> ", 12);
			fflush(stdout); /*flush output buffer*/
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
					write_error(args, loops);
				}
			}
			free(input);
		} /*show prompt*/
		free(input);
	}
	/*else non interactive mode*/
	return (0);
}

