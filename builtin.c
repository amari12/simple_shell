#include "shell.h"

/**
 * _myexit - exits the shell
 * @inf: Struct
 * Return: exits with given exit status
 */

int _myexit(inf_t *inf)
{
	int exitcheck;

	/*If there is an argument provided*/
	if (inf->argv[1])
	{
		/*arg to int*/
		exitcheck = _erratoi(inf->argv[1]);
		/*conversion failed:*/
		if (exitcheck == -1)
		{
			/*print error*/
			inf->status = 2;
			print_error(inf, "Illegal number: ");
			_eputs(inf->argv[1]);
			_eputchar('\n');
			return (1);
		}

		/*Store the converted exit value in inf->err_num*/
		inf->err_num = _erratoi(inf->argv[1]);
		/*indicate that program should exit with specified exit val*/
		return (-2);
	}

	inf->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @inf: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */

int _mycd(inf_t *inf)
{
	char *s, *dir, buffer[1024];
	int chdir_result;

	s = getcwd(buffer, 1024); /*Get current working directory*/
	if (!s) /*failed*/
		_puts("TODO: >>getcwd failure emsg here<<\n");

	if (!inf->argv[1])
	{
		dir = _getenv(inf, "HOME="); /*"HOME" environment variable*/
		if (!dir) /*If "HOME" not set, change val "PWD" or root directory ("/")*/
			chdir_result = chdir((dir = _getenv(inf, "PWD=")) ? dir : "/");
		else /*Change to the directory specified by "HOME"*/
			chdir_result = chdir(dir);
	}
	else if (strcmp(inf->argv[1], "-") == 0)
	{
		if (!_getenv(inf, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		} /*Print the value of "OLDPWD" and change to the directory*/
		_puts(_getenv(inf, "OLDPWD=")), _putchar('\n');
		chdir_result = chdir((dir = _getenv(inf, "OLDPWD=")) ? dir : "/");
	}
	else /*Change to the directory specified by the argument*/
		chdir_result = chdir(inf->argv[1]);

	if (chdir_result == -1)
	{ /*If chdir fails, print an error message*/
		print_error(inf, "can't cd to ");
		_eputs(inf->argv[1]), _eputchar('\n');
	}
	else
	{ /*Update the values of "OLDPWD" and "PWD"*/
		_setenv(inf, "OLDPWD", _getenv(inf, "PWD="));
		_setenv(inf, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes current directory of the process
 * @inf: Struct
 * Return: int (0)
 */

int _myhelp(inf_t *inf)
{
	char **arg_array;

	/*Store the argv array*/
	arg_array = inf->argv;

	/*Print a message*/
	_puts("help call works. Function not yet implemented \n");

	if (0)
		_puts(*arg_array);

	/*out*/
	return (0);
}

