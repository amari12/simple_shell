#include "main.h"

/**
 * get_input - reads the input from user and saves as string
 * Return: char *input
 */

char *get_input(void)
{
	int result;
	char *input_str = NULL;
	size_t size = SIZE;

	/*fgets_result = fgets(input, SIZE, stdin);*/
	result = getline(&input_str, &size, stdin);
	if (result == EOF)
	{
		write(STDOUT_FILENO, "\n", 1);
		free(input_str);
		exit(EXIT_SUCCESS);
	}
	else if (result == -1)
	{
		perror("Getline failed ");
		free(input_str);
		exit(EXIT_FAILURE);
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
	char **list_tokens = NULL;

	list_tokens = malloc((SIZE + 1) * sizeof(char *));
	if (list_tokens == NULL)
	{
		perror("Failed to allocate memory");
		free(list_tokens);
		return;
	}
	/*make copy of input string*/
	strcpy(cp_input, input);
	/*split input string into tokens: args*/
	check_token = strtok(cp_input, " \n");
	/*check_token = strtok(cp_input, " ");*/
	while (check_token != NULL && i < SIZE)
	{
		list_tokens[i] = check_token;
		i++;
		check_token = strtok(NULL, " \n");
	}
	list_tokens[i] = NULL; /*last string in array should be NULL*/
	for (j = 0; j < i; j++)
		args[j] = list_tokens[j];
	args[j] = NULL;
	free(list_tokens);
} /*split input*/

/**
 * forking - forks parent and child process and exe cmd
 * @args: separated input string
 * @cmd: command (args[0])
 * Return: int (0 = no errors; 1 = error)
 */

int forking(char *args[], char *cmd __attribute__((unused)))
{
	pid_t fork_result;
	char *path = NULL;
	int status;
	int pipefd[2];
	char buffer[4069];
	ssize_t bytesrd;
	FILE *stdout_file;
	int stdout_fd;

	path = check_path(args);
	if (path != NULL)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			free(path);
			return (1);
		}
		fork_result = fork(); /*gives a pid*/
		if (fork_result == -1) /*error*/
		{
			perror("Fork failed");
			free(path);
			return (1);
		}
		else if (fork_result == 0) /*success -> child process*/
		{
			close(pipefd[0]);
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			/*close(pipefd[0]);*/
			status = execve(path, args, environ); /*exe cmd*/
			if (status == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else /*parent*/
		{
			close(pipefd[1]);
			stdout_fd = open("stdout.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (stdout_fd == -1)
			{
				perror("open");
				free(path);
				return (1);
			}
			stdout_file = fdopen(stdout_fd, "w");
			if (stdout_file == NULL)
			{
				perror("fdopen");
				close(stdout_fd);
				free(path);
				return (1);
			}
			while ((bytesrd = read(pipefd[0], buffer, sizeof(buffer))) > 0)
			{
				write(STDOUT_FILENO, buffer, bytesrd);
				fwrite(buffer, 1, bytesrd, stdout_file);
			}
			/*close(pipefd[0]);*/
			/*close(pipefd[1]);*/
			fclose(stdout_file);
			do
			{
				if (waitpid(fork_result, &status, WUNTRACED) == -1)
				{
					perror("waitpid");
					free(path);
					return (1);
				}
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	free(path);
	return (1);
} /*forking*/

