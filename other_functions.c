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

	itoa(getpid(), pid);
	strcat(prefix, pid);
	strcat(prefix, "/cmdline");
	file = open(prefix, O_RDONLY);
	if (file != -1)
	{
		rd = read(file, buff, sizeof(prog));
		if (rd != -1)
		{
			prog = malloc(rd * sizeof(char) + 1);
			strncpy(prog, buff, rd);
			prog[rd] = '\0';
		}
	}
	close(file);
	itoa(loops, loops_str);
	write(STDOUT_FILENO, prog, strlen(prog));
	write(STDOUT_FILENO, ": ", 2);
	write(STDOUT_FILENO, loops_str, strlen(loops_str));
	write(STDOUT_FILENO, ": ", 2);

	perror(*args);
	free(prog);
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


