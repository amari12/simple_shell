#include "shell.h"

/**
 * _erratoi - converts a string to an integer
 * @s: str
 * Return: 0 if no numbers in string,
 *	 converted number otherwise
 *       -1 on error
 */

int _erratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	/*If the first character is '+', skip it*/
	if (*s == '+')
		s++;

	for (i = 0;  s[i] != '\0'; i++)
	{
		/*If the character is a digit (0-9)*/
		if (s[i] >= '0' && s[i] <= '9')
		{
			/*process*/
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
		{
			/*Return -1 if the string contains non-digit characters*/
			return (-1);
		}
	}

	return (result);
}

/**
 * print_error - prints an error message
 * @inf: struct
 * @estr: string with error type
 * Return: 0 if no numbers in string,
 *	 converted number otherwise
 *       -1 on error
 */

void print_error(inf_t *inf, char *estr)
{
	/*assemble errir msg*/
	_eputs(inf->fname);
	_eputs(": ");

	print_d(inf->line_count, STDERR_FILENO);
	_eputs(": ");

	_eputs(inf->argv[0]);
	_eputs(": ");

	_eputs(estr);
}

/**
 * print_d - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor
 * Return: number of characters printed
 */

int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	/*Assign the appropriate putchar function*/
	if (fd == STDERR_FILENO)
		__putchar = _eputchar;

	/*Determine the absolute value of the input*/
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;

	current = _abs_;
	/*Iterate through the digits*/
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	/*Print the last digit (ones place)*/
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * convert_number - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 * Return: string
 */

char *convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	/*Check if the number is negative*/
	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}

	/*Determine the array of characters to use*/
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	/*conversion loop*/
	do {
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	/*handle sign*/
	if (sign)
		*--ptr = sign;

	return (ptr);
}

/**
 * remove_comments - function replaces first instance of '#' with '\0'
 * @buffer: address of the string to modify
 * Return: void
 */

void remove_comments(char *buffer)
{
	int i;

	for (i = 0; buffer[i] != '\0'; i++)
	{
		/*check char*/
		if (buffer[i] == '#' && (!i || buffer[i - 1] == ' '))
		{
			/*Replace the '#' character with a null character*/
			buffer[i] = '\0';
			break;
		}
	}
}

