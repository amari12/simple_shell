#include "main.h"

/**
 * loop - main shell loop
 * @inf: info struct
 * @argv: the arguments
 * Return: 0 success, 1 error, else error code
 */

int loop(inf_t *inf, char **argv)
{
	ssize_t status = 0;
	int ibuiltin = 0;

	while (status != -1 && ibuiltin != -2)
	{ /*repeat loop / prompt */
		clear_inf(inf);
		/*check is interactive mode*/
		if (is_interactive(inf))
			_putstr("our shell>> ");
		_eputchar(FLUSH); /*flush stdout*/
		status = get_input(inf);
		if (status != -1)
		{
			set_inf(inf, argv);
			ibuiltin = find_bi(inf);
			if (ibuiltin == -1)
				find_cmd(inf);
		}
		else if (is_interactive(inf))
			_putchar('\n');
		free_inf(inf, 0);
	}
	write_history(inf);
	free_inf(inf, 1);
	if (!is_interactive(inf) && inf->status)
		exit(inf->status);
	if (ibuiltin == -2)
	{
		if (inf->err_num == -1)
			exit(inf->status);
		exit(inf->err_num);
	}
	return (ibuiltin);
}

/**
 * find_bi - finds a builtin command
 * @inf: the parameter & return inf struct
 * Return: int  -1 bi not found,
 *		0 bi executed successfully,
 *		1 bi found but not successful,
 *		-2 bi signals exit()
 */

int find_bi(inf_t *inf)
{
	int i, result = -1;
	builtin_table bi_table[] = {
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

	/*look through bi's and compare cmd*/
	for (i = 0; bi_table[i].type; i++)
		if (strcmp(inf->argv[0], bi_table[i].type) == 0)
		{
			inf->line_count++;
			result = bi_table[i].func(inf);
			break;
		}
	return (result);
}

/**
 * find_cmd - find command in PATH
 * @inf: struct args
 * Return: void
 */

void find_cmd(inf_t *inf)
{
	char *path = NULL;
	int i, j;

	inf->path = inf->argv[0];
	if (inf->linecount_flag == 1)
	{
		inf->line_count++;
		inf->linecount_flag = 0;
	}
	for (i = 0, j = 0; inf->arg[i]; i++)
		if (!is_delim(inf->arg[i], " \t\n"))
			j++;
	if (j == NULL)
		return;
	path = find_path(inf, _getenv(inf, "PATH="), inf->argv[0]);
	if (path != NULL)
	{
		inf->path = path;
		fork_cmd(inf);
	}
	else
	{
		if ((is_interactive(inf) || _getenv(inf, "PATH=")
					|| inf->argv[0][0] == '/') && is_cmd(inf, inf->argv[0]))
			fork_cmd(inf);
		else if (*(inf->arg) != '\n')
		{
			inf->status = 127;
			print_error(inf, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @inf: the parameter & return inf struct
 *
 * Return: void
 */
void fork_cmd(inf_t *inf)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(inf->path, inf->argv, get_environ(inf)) == -1)
		{
			free_inf(inf, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(inf->status));
		if (WIFEXITED(inf->status))
		{
			inf->status = WEXITSTATUS(inf->status);
			if (inf->status == 126)
				print_error(inf, "Permission denied\n");
		}
	}
}

