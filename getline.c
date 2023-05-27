#include "shell.h"

/**
 * input_buffer - bufferfers chained commands
 * @inf: parameter struct
 * @buffer: address of buffer
 * @len: address of len var
 * Return: bytes read
 */

ssize_t input_buffer(inf_t *inf, char **buffer, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		free(*buffer); /*free existing buffer*/
		*buffer = NULL;

		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buffer, &len_p, stdin);
#else
		r = _getline(inf, buffer, &len_p);
#endif

		if (r > 0)
		{
			if ((*buffer)[r - 1] == '\n')
			{
				(*buffer)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}

			inf->linecount_flag = 1;
			remove_comments(*buffer);
			build_history_list(inf, *buffer, inf->histcount++);
			if (strchr(*buffer, ';'))
			{ /*Check if the input contains a command chain*/
				*len = r;
				inf->cmd_buf = buffer;
			}
		}
	}

	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @inf: struct
 * Return: bytes read
 */

ssize_t get_input(inf_t *inf)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buffer_p = &(inf->arg), *p;

	_putchar(FLUSH);
	r = input_buffer(inf, &buffer, &len);
	if (r == -1) /* EOF */
		return (-1);

	if (len)
	{
		j = i;
		p = buffer + i; /* get pointer for return */
		/*handle cmd chain*/
		check_chain(inf, buffer, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(inf, buffer, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer */
		{
			i = len = 0; /* reset position and length */
			inf->cmd_buf_type = CMD_NORM;
		}

		*buffer_p = p;
		return (strlen(p));
	}

	*buffer_p = buffer; /* else not a chain*/

	return (r);
}

/**
 * read_buf - reads a buffer
 * @inf: struct
 * @buffer: bufferfer
 * @i: size
 * Return: r (read bytes)
 */

ssize_t read_buf(inf_t *inf, char *buffer, size_t *i)
{
	ssize_t r = 0;

	/*If there are remaining bytes*/
	if (*i)
		return (0);

	/*read data*/
	r = read(inf->readfd, buffer, READ_SIZE);
	if (r >= 0) /*read fialed*/
		*i = r;

	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @inf: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(inf_t *inf, char **ptr, size_t *length)
{
	static char buffer[READ_SIZE];
	static size_t i, len;
	size_t m;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr; /*current buffer ptr*/
	/*checks:*/
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	/*read data*/
	r = read_buf(inf, buffer, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = strchr(buffer + i, '\n'); /*find new line char*/
	m = c ? 1 + (unsigned int)(c - buffer) : len;
	/*memory*/
	new_p = _realloc(p, s, s ? s + m : m + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s) /*add str to buffer*/
		strncat(new_p, buffer + i, m - i);
	else
		strncpy(new_p, buffer + i, m - i + 1);

	/*process*/
	s += m - i;
	i = m;
	p = new_p;
	/*update info*/
	if (length)
		*length = s;
	*ptr = p;

	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: signal number
 * Return: void
 */

void sigintHandler(__attribute__((unused))int sig_num)
{
	/*new line*/
	_putstr("\n");
	_putstr("$ ");

	/*fluch stdout*/
	_putchar(FLUSH);
}

