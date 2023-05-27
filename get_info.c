#include "shell.h"

/**
 * clear_inf - initializes inf_t struct
 * @inf: struct ptr
 * Return: void
 */

void clear_inf(inf_t *inf)
{
	/*initialse struct*/
	inf->arg = NULL;
	inf->argv = NULL;
	inf->path = NULL;
	inf->argc = 0;
}

/**
 * set_inf - initializes inf_t struct
 * @inf: struct ptr
 * @argv: argument vector
 * Return: void
 */

void set_inf(inf_t *inf, char **argv)
{
	int i = 0;

	inf->fname = argv[0];
	/*check for additional args*/
	if (inf->arg)
	{
		/*split input*/
		inf->argv = _strtok(inf->arg, " \t");
		if (!inf->argv) /*tokenisation failed*/
		{
			/*memory allocation*/
			inf->argv = malloc(sizeof(char *) * 2);
			if (inf->argv)
			{ /*assign arguments*/
				inf->argv[0] = strdup(inf->arg);
				inf->argv[1] = NULL;
			}
		}
		/*count args*/
		for (i = 0; inf->argv && inf->argv[i]; i++)
		{
			;
		}
		inf->argc = i;
		/*Replace aliases with their corresponding values*/
		replace_alias(inf);
		/*Replace aliases with their corresponding values*/
		replace_vars(inf);
	}
}

/**
 * free_inf - frees inf_t struct fields
 * @inf: struct ptr
 * @all: true if freeing all successful
 */

void free_inf(inf_t *inf, int all)
{
	/*free memory*/
	ffree(inf->argv);
	/*reset*/
	inf->argv = NULL;
	inf->path = NULL;
	if (all) /*flag = true*/
	{
		/*If inf->cmd_buf is not allocated (NULL)*/
		if (!inf->cmd_buf)
			free(inf->arg);
		if (inf->env) /*inf->env is allocated (not NULL)*/
			free_list(&(inf->env));
		if (inf->history) /*inf->history is allocated (not NULL)*/
			free_list(&(inf->history));
		if (inf->alias) /*inf->alias is allocated (not NULL)*/
			free_list(&(inf->alias));
		ffree(inf->environ); /*environ*/
			inf->environ = NULL;
		bfree((void **)inf->cmd_buf);
		/*If inf->readfd is greater than 2 (file descriptor)*/
		if (inf->readfd > 2)
			close(inf->readfd);
		/*flush output buffer*/
		_putchar(FLUSH);
	}
}

