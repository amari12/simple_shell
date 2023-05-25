#include "main.h"

/**
 * get_input - reads the input from user and saves as string
 * 
 * Return: char *input
 */

char *get_input(void)
{
	int result;
	char *input_str = NULL;
	size_t size = SIZE;

	/*fgets_result = fgets(input, SIZE, stdin);*/
	result = getline(&input_str, &size, stdin);
	if (result == -1) /*error*/
	{
		perror("Getline failed ");
		free(input_str);
		exit(EXIT_FAILURE);
	}
	if (result == EOF) /*end of file*/
	{
		write(STDOUT_FILENO, "\n", 1);
		free(input_str);
		exit(EXIT_SUCCESS);
	}
	
	if (check_spaces(input_str) == 0) /*empty string/spaces*/
	{
		free(input_str);
		input_str = NULL;
	}
	return (input_str);
} /*get input*/

/**
 * get_input2 - reads the input using fgets()
 * @input: input string
 * Return: void
 */

void get_input2(char **input)
{
	static char buffer[SIZE];
	static int index, read_size;
	char *temp;
	int newline_i, j;

	*input = NULL; /*initialise*/
	while (1) /*infinite loop until return called*/
	{
		if (index >= read_size) /*if buffer empty, read input*/
		{
			read_size = read(STDIN_FILENO, buffer, SIZE);
			if (read_size == 0) /*EOF*/
				break; /*handled in main.c*/
			else if (read_size < 0)
			{ /*error reading input*/
				perror("Error reading input");
				exit(EXIT_FAILURE);
			}
			index = 0;
		}
		for (newline_i = index; newline_i < read_size; newline_i++)
		{ /*find new line char */
			if (buffer[newline_i] == '\n')
				break; /*i shows index of new line*/
		}
		temp = realloc(*input, (newline_i - index + 1) * sizeof(char));
		if (temp == NULL)
		{
			perror("Memory allocation failed");
			exit(EXIT_FAILURE);
		}
		*input = temp;
		for (j = 0; index + j < newline_i; j++) /*add content of buffer to input*/
			(*input)[j] = buffer[index + j];
		(*input)[j] = '\0'; /*null terminator at end*/
		index = newline_i + 1; /*move index to next char after newline char*/
		if (newline_i < read_size ||
				(newline_i == read_size && buffer[newline_i - 1] == '\n'))
			return;
	} /*while*/
} /*get_input 2*/

/**
 * split_input - splits input string into separate arguments
 * @input: input string
 * @args: separated input string
 * Return: void
 */

void split_input(char *input, char *args[])
{
	int i = 0, j;
	char cp_input[SIZE + 1];
	char *check_token;
	char **list_tokens;

	list_tokens = malloc(SIZE * sizeof(char *));
	if (list_tokens == NULL)
	{
		free(list_tokens);
		return;
	}
	/*make copy of input string*/
	strcpy(cp_input, input);
	/*split input string into tokens: args*/
	check_token = strtok(cp_input, " \n");
	/*check_token = strtok(cp_input, " ");*/
	while (check_token != NULL)
	{
		list_tokens[i] = check_token;
		i++;
		check_token = strtok(NULL, " \n");
	}
	args[i] = NULL; /*last string in array should be NULL*/
	for (j = 0; j <= i; j++)
		args[j] = list_tokens[j];
} /*split input*/

/**
 * forking - forks parent and child process and exe cmd
 * @input: input string
 * @args: separated input string
 * @cmd: command (args[0])
 * @path: path
 * @prog_name: program name
 * Return: int (0 = no errors; 1 = error)
 */

int forking(char *args[], char *cmd)
{
	pid_t fork_result;
	char *path = NULL;
	int status, error = 0;

	(void) cmd;
	path = check_path(args);
	if (path != NULL)
	{
		fork_result = fork(); /*gives a pid*/
		if (fork_result == -1) /*error*/
		{
			perror("Fork failed");
			/*exit(EXIT_FAILURE);*/
		} /*fail*/
		else if (fork_result == 0) /*success -> child process*/
		{
			status = execve(path, args, environ); /*exe cmd*/
			if (status == -1)
				error = 1;
			/*perror("execve failed");*/ /*if exe fails*/
			/*exit(EXIT_FAILURE);*/
		} /*child*/
		else /*parent*/ /*******************************/
			wait(&status);
	}
	free(path);
	return (error);
} /*forking*/

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
				return(NULL);
			}
			strcpy(result, directory);
			strcat(result, "/");
			strcat(result, (char *) cmd);
			if (access(result, X_OK) == 0)
			{
				free(cp_path);
				return(result);
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

