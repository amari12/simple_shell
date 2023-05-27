#include "shell.h"

/**
 *_putstr - prints an input string
 * @str: string
 * Return: void
 */

void _putstr(char *str)
{
	int i = 0;

	/*check if empty*/
	if (!str)
		return;

	/*write using _putchar*/
	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i++;
	}
}

/**
 * _putchar - writes the character c to stdout
 * @c: char
 * Return: success 1, error -1
 */

int _putchar(char c)
{
	static int i;
	static char buf[WRITE_SIZE];

	/*checks:*/
	if (c == FLUSH || i >= WRITE_SIZE)
	{
		/*write*/
		write(1, buf, i);
		i = 0;
	}

	/*not flushing stdout*/
	if (c != FLUSH)
		buf[i++] = c;

	return (1);
}

/**
 * starts_with - checks if needle starts with haystack
 * @str: string to search
 * @find: the substring to find
 * Return: address of char / NULL
 */

char *starts_with(const char *str, const char *find)
{
	/*search loop*/
	while (*find)
	{
		if (*find++ != *str++)
			return (NULL); /*not found*/
	}

	return ((char *)str);
}

