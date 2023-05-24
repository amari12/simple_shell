#include "main.h"

/**
 * handle_builtins - handles builtin functions
 * @args: input string separated into arguments
 * @environ: environmental variables
 * Return: int (check to flag if env was called)
 */

int handle_builtins(char *args[], char **environ)
{
	int check = 0;
	int status, i = 0;

	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1] != NULL)
			status = atoi(args[1]); /*will save as status in main*/
		else
			status = EXIT_SUCCESS;
		exit(status);
		/*exit(EXIT_SUCCESS);*/
	}
	else if (strcmp(args[0], "env") == 0)
	{
		while (environ[i] != NULL)
		{
			/*printf("%s\n", *env);*/
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
			i++;
		}
		/*instead of conitnue*/
		check = 1;
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		change_dir(args);
		check = 1;
	}
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
	/*printf("prev: %s\n", prev);*/

	if (args[1] == NULL || strcmp(args[1], "~") == 0)
		change = home;
	else if (strcmp(args[1], "-") == 0)
		change = prev;
	else
		change = args[1];

	if (change == NULL)
	{
		perror("No directory found");
		return;
	}
	/*printf("Change to dir: %s\n", change);*/
	result = chdir(change); /*change directory*/
	if (result != 0)
	{
		perror("cd failed");
		return;
	}
	/*update env var (PWD) with current dir*/
	get_result = getcwd(current, sizeof(current));
	if (get_result != NULL)
		setenv("PWD", current, 1);
	else
		perror("Failed to update current directory");
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

