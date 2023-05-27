#include "shell.h"

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

