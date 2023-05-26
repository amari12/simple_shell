#include "main.h"

/**
 * input_buffer - creates buffer for chained commands
 * @inf: struct args
 * @buffer: buffer
 * @len: length
 * Return: ssize_t bytes read
 */

ssize_t input_buffer(inf_t *inf, char **buffer, size_t *len)
{
	ssize_t result = 0;
	size_t len_p = 0;

	if (!*len) /* nothing left in the buffer */
	{
		/*memory free*/
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigintHandler);
		/*different getline functs*/
		/*result = getline(buffer, &len_p, stdin);*/
		result = _getline(inf, buffer, &len_p);
		if (result > 0)
		{
			if ((*buffer)[result - 1] == '\n')
			{ /*deal with new line char*/
				(*buffer)[result - 1] = '\0';
				result--;
			}
			/*inf struct*/
			inf->linecount_flag = 1;
			/*handle commnets - task */
			remove_comments(*buffer);
			build_history_list(inf, *buffer, inf->h_count++);
			if (strchr(*buffer, ';'))
			{
				*len = result;
				inf->cmd_buffer = buffer;
			}
		} /*if*/
	}/*if (!*len)*/
	return (result);
}

/**
 * get_input - gets a line excluding the newline
 * @inf: struct args
 * Return: ssize_t bytes read
 */

ssize_t get_input(inf_t *inf)
{
	static char *buffer;
	static size_t i, j, len;
	ssize_t result = 0;
	char **buffer_ptr = &(inf->arg), *p;

	_putchar(FLUSH);
	result = input_buffer(inf, &buffer, &len);
	if (result == -1) /* EOF */
		return (-1);
	if (len)
	{
		j = i;
		/*ptr for return*/
		p = buffer + i;
		/*handle cmd chain*/
		check_chain(inf, buffer, &j, i, len);
		while (j < len) /* until semicolon or end */
		{
			if (is_chain(inf, buffer, &j))
				break;
			j++;
		}
		i = j + 1;
		if (i >= len) /* end of buffer */
		{
			i = len = 0; /*reset*/
			inf->cmd_buffer_type = CMD_NORM;
		}
		*buffer_ptr = p;
		return (strlen(p));
	}
	*buffer_ptr = buffer; /* else not a chain, pass back buffer from _getline() */
	return (result); /* length */
}

/**
 * read_buffer - read buffer
 * @inf: struct args
 * @buffer: buffer
 * @i: size
 * Return: ssize_t
 */

ssize_t read_buffer(inf_t *inf, char *buffer, size_t *i)
{
	ssize_t rd = 0;

	if (*i) /*if buffer not empty*/
		return (0); /*no new data read*/
	rd = read(inf->readfd, buffer, READ_SIZE);
	if (rd >= 0) /*success*/
		*i = rd;
	/* return bytes read*/
	return (rd);
}

/**
 * _getline - gets line of input from STDIN
 * @inf: struct args
 * @ptr: address 
 * @length: size of ptr buffer
 * Return: int s
 */

int _getline(inf_t *inf, char **ptr, size_t *length)
{
	static char buffer[READ_SIZE];
	static size_t i, len;
	size_t line_len;
	ssize_t result = 0;
	char *line_p = NULL, *new_p = NULL, *end;

	line_p = *ptr; /*assign*/
	if (line_p != NULL && length != NULL)
		line_len = *length;
	if (i == len)
	{
		i = len = 0;
	}
	result = read_buffer(inf, buffer, &len);
	if (result == -1 || (result == 0 && len == 0))
		return (-1); /*check is data was read*/
	end = strchr(buffer + i, '\n'); /*find new line*/
	if (end == NULL)
		line_len = end - (buffer + i) + 1;
	else /*no new line char*/
		line_len = len - i;
	new_p = realloc(line_p,(line_len + 1) * sizeof(char));
	if (new_p == NULL) /*if realloc failed*/
	{
		if (line_p != NULL)
			free(line_p);
		return (-1);
	}
	strncpy(new_p, buffer + i, line_len);
	new_p[line_len] = '\0';
	i += line_len;
	line_p = new_p;
	if (length != NULL)
		*length = line_len;
	*ptr = line_p;
	return (line_len); /*len of str*/
}

/**
 * sigintHandler - handles ctrl-C
 * @sig_num: signal number
 * Return: void
 */

void sigintHandler(__attribute__((unused))int sig_num)
{
	_putstr("\n");
	_putstr("$ ");
	_putchar(FLUSH);
}

