#include "main.h"

/**
 * **_strtok1 - splits a string into words
 * repeat delimiters ignored
 * @str: input string
 * @d: delimeter string
 * Return: ptr -> arr of strings, NULL = failure
 */

char **_strtok1(char *str, char *d)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0) /*check input str*/
		return (NULL);
	if (d == NULL) /*empty list for deliminators*/
		d = " ";
	/*Count the number of words in the input string based on the delimiters*/
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delim(str[i], d) && (is_delim(str[i + 1], d) || !str[i + 1]))
			numwords++;
	if (numwords == 0) /*no words found*/
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (s == NULL) /*malloc failed*/
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{ /*tokenise*/
		while (is_delim(str[i], d)) /*skip delims*/
			i++;
		k = 0;
		while (!is_delim(str[i + k], d) && str[i + k])
			k++; /*cpy chars of word*/
		s[j] = malloc((k + 1) * sizeof(char));
		if (s[j] != NULL) /*malloc failed*/
		{
			for (k = 0; k < j; k++) /*free prev*/
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0; /*add null terminator*/
	}
	s[j] = NULL; /*add null ptr*/
	return (s); /*return arr of char pointers -> words*/
}

/**
 * **_strtok2 - splits a string into words
 * @str: input string
 * @d: delimeter
 * Return: pointer -> arr of strings, NULL - failure
 */

char **_strtok2(char *str, char d)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
				(str[i] != d && !str[i + 1]) || str[i + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (str[i] == d && str[i] != d)
			i++;
		k = 0;
		while (str[i + k] != d && str[i + k] && str[i + k] != d)
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}

