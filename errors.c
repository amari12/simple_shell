#include "main.h"

/**
 *_eputs - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes the character to stderr
 * @c: char
 * Return: int (1 success, -1 fail)
 */

int _eputchar(char c)
{
	static int i;
	static char buffer[WRITE_SIZE];

	/*If the character is FLUSH or the buffer is full*/
	if (c == FLUSH || i >= WRITE_SIZE)
	{
		/*Write the buffer contents to the */
		/*standard error stream (file descriptor 2)*/
		write(2, buffer, i);
		i = 0;
	}
	if (c !=  FLUSH)
		buffer[i++] = c;
	return (1); /*success*/
}

/**
 * _putfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static int i;
	static char buf[WRITE_SIZE];

	if (c ==  FLUSH || i >= WRITE_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c !=  FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 *_putsfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd);
	}
	return (i);
}

