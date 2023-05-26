#include "shell.h"

/**
 * is_interactive - checks if shell is interactive mode
 * @inf: struct
 * Return: 1 = interactive mode, 0 other
 */

int is_interactive(inf_t *inf)
{
	return (isatty(STDIN_FILENO) && inf->readfd <= 2);
}

/**
 * is_delim - checks if char is a valid delimeter
 * @c: char
 * @d: delimeter string
 * Return: int (1 true, 0 false)
 */

int is_delim(char c, char *d)
{
	/*look through delim str*/
	while (*d)
		if (*d++ == c)
			return (1); /*found*/
	/*did not find = 0*/
	return (0);
}

/**
 *_isalpha - checks if alphabetic character
 *@c: char
 *Return: int - 1 if c is alphabetic, 0 not
 */

int _isalpha(int c)
{
	/*check if alphabetic char*/
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else /*not alpha char*/
		return (0);
}

/**
 *_atoi - convertsstr to int
 *@s: str
 *Return: 0 if no numbers in string, converted number otherwise
 */

int _atoi(char *s)
{
	int i, sign = 1, flag = 0, out;
	unsigned int result = 0;

	/*loop*/
	for (i = 0;  s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;
		/*check if digit*/
		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			/*convert*/
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}
	/*incorporate sign*/
	if (sign == -1)
		out = -result;
	else
		out = result;
	return (out);
}

