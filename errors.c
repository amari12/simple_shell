#include "shell.h"

/**
 *_eputs - prints an input string
 * @str: str
 * Return: void
 */

void _eputs(char *str)
{
	int i = 0;

	/*check if empty*/
	if (str == NULL)
		return;
	/*loop*/
	while (str[i] != '\0')
	{
		/*writes*/
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes the character c to stderr
 * @c: char
 * Return: success 1, error -1
 */

int _eputchar(char c)
{
	static int i;
	static char buf[WRITE_SIZE];

	/*If the character is FLUSH or the buffer is full*/
	if (c == FLUSH || i >= WRITE_SIZE)
	{
		/*Write the buffer contents to the */
		/*standard error stream (file descriptor 2)*/
		write(2, buf, i);
		i = 0;
	}
	if (c != FLUSH)
		buf[i++] = c;
	/*success*/
	return (1);
}

/**
 * _putfd - writes the character c to given fd
 * @c: char
 * @fd:  filedescriptor to write to
 * Return: On success 1, error -1
 */

int _putfd(char c, int fd)
{
	static int i;
	static char buf[WRITE_SIZE];

	/* the char = FLUSH marker or the buffer is full*/
	if (c == FLUSH || i >= WRITE_SIZE)
	{
		/*Write contents of buffer to file descriptor*/
		write(fd, buf, i);
		i = 0;
	}
	/*not fluch marker*/
	/*Store the character in the buffer and increment the index*/
	if (c != FLUSH)
		buf[i++] = c;
	return (1); /*success*/
}

/**
 *_putsfd - prints an input string
 * @str: string
 * @fd: the filedescriptor to write to
 * Return: the number of chars printed
 */

int _putsfd(char *str, int fd)
{
	int i = 0;

	if (str == NULL) /*no chars to print*/
		return (0);
	/*chars to print*/
	while (*str)
	{
		/*write each character tofd*/
		i += _putfd(*str++, fd);
	}
	return (i); /*success - chars printed*/
}

