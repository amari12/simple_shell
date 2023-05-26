#include "main.h"

/**
 * _putstr - prints a str
 * @str: string
 * Return: void
 */

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

/**
 * _strcpy - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	if (dest == src || src == 0)
		return (dest);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

/**
 * _strdup - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *_strdup(const char *str)
{
	int length = 0;
	char *ret;

	if (str == NULL)
		return (NULL);
	while (*str++)
		length++;
	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
		return (NULL);
	for (length++; length--;)
		ret[length] = *--str;
	return (ret);
}

