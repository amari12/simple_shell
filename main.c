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
	char *input = NULL;
	char *exit_msg = "EOF\n...\n";
	char *cmd;
	char *args[ARGS_SIZE];
	int exit_loop = 0;
	char *path; /*, *temp;*/
	/*int check;*/ /*for builtin, check for env*/
	/*char **environ = __environ;*/ /*global variable*/

	path = getenv("PATH"); /*get path*/
	while (exit_loop != 1) /*shell loop*/
	{
		printf("our shell>> "); /*prompt*/
		fflush(stdout); /*flush output buffer*/
		get_input(&input); /*read input*/
		/*get_input2(&input);*/ /*advanced*/
		if (input == NULL)
		{ /*eof*/
			exit_loop = 1; /*will exit loop*/
			write(STDOUT_FILENO, exit_msg, strlen(exit_msg));
			/*write(STDOUT_FILENO, &newline, 1);*/
			break;
		}
		else if (input != NULL)
		{
			if (check_spaces(input) == 1)
			{ /*check if only spaces were entered*/
				check_comments(input); /*search & handle comments*/
				split_input(input, args); /*separate input -> args list*/
				cmd = args[0];
				/*check =*/
				if (handle_builtins(args, environ, argv[0]) == 1) /*handle builtins*/
					continue; /*env builtin or cd -> restart loop*/
				forking(args, cmd, path, argv[0]); /*fork and exe child process*/
			}
		}
		free(input);
	} /*show prompt*/
	free(input);
	return (0);
}

