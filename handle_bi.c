#include "main.h"

/**
 * handle_builtins - handles builtin functions
 * @args: input string separated into arguments
 * Return: int (check to flag if env was called)
 */

int handle_builtins(char *args[])
{
	int check = 0, status, i;
	char *env;
	char *parts[] = {"USER", "LANGUAGE", "SESSION", "COMPIZ_CONFIG_PROFILE",
		"SHLVL", "HOME", "C_IS", "DESKTOP_SESSION", "LOGNAME",
		"TERM", "PATH", "DISPLAY", "NULL"};

	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1] != NULL)
			status = atoi(args[1]);
		/*******/
		else
			status = EXIT_SUCCESS;
		/*free(args);*/
		exit(status);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; parts[i] != NULL; i++)
		{
			env = getenv(parts[i]);
			if (env != NULL)
			{
				write(STDOUT_FILENO, parts[i], strlen(parts[i]));
				write(STDOUT_FILENO, "=", 1);
				write(STDOUT_FILENO, env, strlen(env));
				write(STDOUT_FILENO, "\n", 1);
			}
		}
		/*args = NULL;*/
		check = 1;
	}
	/*else if (strcmp(args[0], "cd") == 0)*/
	/*{*/
	/*	change_dir(args);*/
	/*	check = 1;*/
	/*}*/
	return (check);
}

/**
 * change_dir - cd builtin
 * @args: input arguments
 * Return: void
 */

void change_dir(char *args[])
{
	char *home, *prev, *get_result, *change;
	char current[SIZE];
	int result;

	home = getenv("HOME"); /*assign variables*/
	prev = getenv("PWD");

	if (args[1] == NULL || strcmp(args[1], "~") == 0)
		change = home;
	else if (strcmp(args[1], "-") == 0)
		change = prev;
	else
		change = args[1];

	if (change == NULL)
	{
		/*perror("No directory found");*/
		/*fprintf(stderr, "%s: no directory found\n", prog_name);*/
		return;
	}
	/*printf("Change to dir: %s\n", change);*/
	result = chdir(change); /*change directory*/
	if (result != 0)
	{
		perror("chdir failed");
	/*	fprintf(stderr, "%s: cd failed\n", prog_name);*/
		return;
	}
	/*update env var (PWD) with current dir*/
	get_result = getcwd(current, sizeof(current));
	if (get_result != NULL)
		setenv("PWD", current, 1);
	else
		fprintf(stderr, "Failed to update current directory\n");
	/*perror("Failed to update current directory");*/
	/*printf("current dir: %s\n", current);*/
}

/**
 * check_comments - checks for comments and removes them
 * @input: input string
 * Return: void
 */

void check_comments(char *input)
{
	size_t i;
	int found = -1;

	if (input == NULL)
		return;
	for (i = 0; i < strlen(input); i++)
	{ /*look for comment #*/
		if (input[i] == '#')
		{
			found = i;
			break;
		}
	}
	if (found != -1)
	{ /*comment found*/
		input[found] = '\0';
	} /*terminate string at #*/
}

