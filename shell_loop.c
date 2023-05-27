#include "shell.h"

/**
 * loop - main shell loop
 * @inf: struct
 * @argv: argument vector from main()
 * Return: 0 success, 1 error / error code
 */

int loop(inf_t *inf, char **argv)
{
	ssize_t r = 0;
	int output = 0;

	while (r != -1 && output != -2)
	{ /*MAIN LOOP*/
		clear_inf(inf);
		/*prompt*/
		if (is_interactive(inf))
			_puts("$ ");
		_eputchar(FLUSH);

		r = get_input(inf); /*get user input*/
		if (r != -1)
		{
			set_inf(inf, argv);
			output = find_builtin(inf);
			if (output == -1)
				find_cmd(inf);
		}
		else if (is_interactive(inf)) /*interactive mode*/
			_putchar('\n');

		free_inf(inf, 0);
	} /*end of loop*/
	write_history(inf); /*write cmd to hist file*/
	free_inf(inf, 1);
	/*handle exit based on loop result*/
	if (!is_interactive(inf) && inf->status)
		exit(inf->status);
	if (output == -2)
	{
		if (inf->err_num == -1)
			exit(inf->status);
		exit(inf->err_num);
	}
	return (output);
}

/**
 * find_builtin - finds a builtin command
 * @inf: struct
 * Return: -1 builtin not found,
 *	0 builtin executed successfully,
 *	1 builtin found but not successful,
 *	-2 builtin signals exit()
 */

int find_builtin(inf_t *inf)
{
	int i, out = -1;
	/*array of bi cmds*/
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	/*iterate throug*/
	for (i = 0; builtintbl[i].type; i++)
	{
		if (strcmp(inf->argv[0], builtintbl[i].type) == 0)
		{
			inf->line_count++;
			/*Execute the corresponding built-in function*/
			out = builtintbl[i].func(inf);
			break;
		}
	}
	/*output*/
	return (out);
}

/**
 * find_cmd - finds a command in PATH
 * @inf: struct
 * Return: void
 */

void find_cmd(inf_t *inf)
{
	char *path = NULL;
	int i, k;

	inf->path = inf->argv[0];
	/*Increment line count if linecount_flag is set*/
	if (inf->linecount_flag == 1)
	{
		inf->line_count++;
		inf->linecount_flag = 0;
	}

	/*Count the number of non-delimiter arguments*/
	for (i = 0, k = 0; inf->arg[i]; i++)
	{
		if (!is_delim(inf->arg[i], " \t\n"))
			k++;
	}
	if (!k) /*no args*/
		return;

	/*Find the executable path using the PATH*/
	path = find_path(inf, _getenv(inf, "PATH="), inf->argv[0]);
	if (path)
	{ /*found path*/
		inf->path = path;
		fork_cmd(inf);
	}
	else
	{ /*path is not found*/
		if ((is_interactive(inf) || _getenv(inf, "PATH=")
			|| inf->argv[0][0] == '/') && is_cmd(inf, inf->argv[0]))
		{ /*If it is an absolute or relative path..*/
			fork_cmd(inf);
		}
		else if (*(inf->arg) != '\n')
		{ /*set the status to 127 and print an error message*/
			inf->status = 127;
			print_error(inf, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @inf: struct
 * Return: void
 */

void fork_cmd(inf_t *inf)
{
	pid_t child_pid;

	/*fork child process*/
	child_pid = fork();

	if (child_pid == -1)
	{ /*failed*/
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{ /*child process*/
		/*execute cmd*/
		if (execve(inf->path, inf->argv, get_environ(inf)) == -1)
		{ /*execve fails*/
			free_inf(inf, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{ /*parent process*/
		wait(&(inf->status));
		/*Check if the child process exited normally:*/
		if (WIFEXITED(inf->status))
		{
			inf->status = WEXITSTATUS(inf->status);
			if (inf->status == 126)
			{
				print_error(inf, "Permission denied\n");
			}
		}
	}
}

