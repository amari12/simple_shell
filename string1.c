#include "main.h"

void _putstr(char *str)
{
	int i = 0;

	/*check if empty*/
	if (str == NULL)
		return;
	/*write*/
	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i++;
	}
}

/**
 * _putchar - writes character to stdout
 * @c: character to print
 * Return: success 1, error -1
 * errno is set
 */

int _putchar(char c)
{
	static int i;
	static char buffer[WRITE_SIZE];

	/*checks*/
	if (c == FLUSH || i >= WRITE_SIZE)
	{
		write(1, buffer, i);
		i = 0;
	}
	if (c != FLUSH)
		buffer[i++] = c;
	return (1);
}

