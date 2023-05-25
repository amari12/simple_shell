#include "main.h"

/**
 * itoa - truns nr to str
 * @nr: number
 * @str: string
 * Return: void
 */

void itoa(int nr, char *str)
{
	int i, k;
	char temp;

	for (i = 0; nr != 0; i++)
	{
		str[i] = '0' + (nr % 10);
		nr = nr / 10;
	}
	for (k = 0; k < i / 2; k++)
	{
		temp = str[k];
		str[k] = str[i - k - 1];
		str[i - k - 1] = temp;
	}
	str[i] = '\0';

}

