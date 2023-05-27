#include "shell.h"

/**
 * _memset - fills memory with a constant byte
 * @s: memory area ptr
 * @b: byte to fill *s with
 * @n: amount of bytes to fill
 * Return: (s) a pointer to the memory area s
 */

char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	/*assign*/
	for (i = 0; i < n; i++)
	{
		s[i] = b;
	}

	return (s);
}

/**
 * ffree - frees a string of strings
 * @pp: string of strings
 * Return: void
 */

void ffree(char **pp)
{
	char **a = pp;

	/*check:*/
	if (!pp) /*empty*/
		return;

	/*loop through not empty*/
	while (*pp)
	{
		free(*pp++);
	}

	free(a);
}

/**
 * _realloc - reallocates a block of memory
 * @ptr: pointer to previous memory block
 * @old_size: byte size of prev block
 * @new_size: byte size of new block
 * Return: pointer
 */

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	/*checks:*/
	if (ptr == NULL) /*empty*/
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	/*memory*/
	p = malloc(new_size);
	if (p == NULL) /*fail*/
		return (NULL);

	/*process*/
	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];

	free(ptr);
	/*output*/
	return (p);
}

