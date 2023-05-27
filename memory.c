#include "shell.h"

/**
 * bfree - frees a pointer and NULLs the address
 * @ptr: address of the pointer to free
 * Return: 1 success, error 0.
 */

int bfree(void **ptr)
{
	/*check if empty*/
	if (ptr && *ptr)
	{
		/*free needed*/
		free(*ptr);
		*ptr = NULL;
		return (1);
	}

	/*default fail*/
	return (0);
}

