#include "main.h"

/**
 * write_error - handles errors
 * @args: arguments
 * @loops: nr of loops executed
 * Return: void
 */

void write_error(char *args[], int loops)
{
	char *prog = NULL;
	char buff[SIZE], pid[50], prefix[7] = "/proc/",
		loops_str[50];
	int file, rd;
	char output[SIZE], err_msg[SIZE];

	sprintf(pid, "%d", getpid());
	strcat(prefix, pid);
	strcat(prefix, "/cmdline");
	file = open(prefix, O_RDONLY);
	if (file != -1)
	{
		rd = read(file, buff, sizeof(buff));
		if (rd != -1)
		{
			buff[rd] = '\0';
			prog = malloc(rd + 1);
			strncpy(prog, buff, rd);
			prog[rd] = '\0';
		}
		close(file);
	}
	sprintf(loops_str, "%d", loops);
	snprintf(output, SIZE, "%s: %s: ", prog, loops_str);
/*	write(STDOUT_FILENO, prog, strlen(prog));*/
/*	write(STDOUT_FILENO, ": ", 2);*/
/*	write(STDOUT_FILENO, loops_str, strlen(loops_str));*/
/*	write(STDOUT_FILENO, ": ", 2);*/
	if (args[0] != NULL)
	{
		snprintf(err_msg, SIZE, "%s: %s\n", args[0], strerror(errno));
		strcat(output, err_msg);
		/*perror(args[0]);*/
	}
	free(prog);
	write(STDOUT_FILENO, output, strlen(output));
}

/**
 * check_path - gets the path and checks if the cmd is valid
 * @args: arguments
 * Return: char * (NULL if not viable, path)
 */

char *check_path(char *args[])
{
	char *result = NULL;
	char *cmd = args[0];
	char *path, *cp_path, *directory;

	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
		{
			result = strdup((char *) cmd);
			return (result);
		}
	}
	else
	{
		path = getenv("PATH");
		cp_path = strdup(path);
		directory = strtok(cp_path, ":");
		while (directory != NULL)
		{
			result = malloc(strlen(directory) + strlen(cmd) + 2);
			if (result == NULL)
			{
				free(result);
				return (NULL);
			}
			strcpy(result, directory);
			strcat(result, "/");
			strcat(result, (char *) cmd);
			if (access(result, X_OK) == 0)
			{
				free(cp_path);
				return (result);
			}
			free(result);
			directory = strtok(NULL, ":");
		}
		free(cp_path);
	}
	return (NULL);
}


/**
 * check_spaces - checks if the whole input is just spaces
 * @input: input string
 * Return: int (1 = found chars, 0 = just spaces)
 */

int check_spaces(char *input)
{
	int i;
	int not_space = 0;

	for (i = 0; input[i] != '\n'; i++)
	{
		if (input[i] != ' ' && input[i] != '\t')
		not_space = 1;
	}
	return (not_space);
}


