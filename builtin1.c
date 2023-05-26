#include "shell.h"

/**
 * _myhistory - displays the history list
 * @inf: args
 * Return: Always 0
 */

int _myhistory(inf_t *inf)
{
	/*print struct*/
	print_list(inf->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @inf: struct
 * @str: str
 * Return: 0 success, 1 error
 */

int unset_alias(inf_t *inf, char *str)
{
	char *p, c;
	int result;

	p = strchr(str, '=');
	if (p == NULL)
		return (1);
	/*switch*/
	c = *p;
	*p = 0;
	result = delete_node_at_index(&(inf->alias),
		get_node_index(inf->alias, node_starts_with(inf->alias, str, -1)));
	*p = c;
	return (result);
}

/**
 * set_alias - sets alias to string
 * @inf: struct
 * @str: str
 * Return: 0 success, 1 error
 */

int set_alias(inf_t *inf, char *str)
{
	char *p;

	p = strchr(str, '=');
	/*check*/
	if (p == NULL)
		return (1);
	if (!*++p)
		return (unset_alias(inf, str));
	unset_alias(inf, str);
	return (add_node_end(&(inf->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: node for alias
 * Return: 0 success, 1 error
 */

int print_alias(list_t *node)
{
	char *p = NULL, *m = NULL;

	if (node) /*not null*/
	{
		p = strchr(node->str, '=');
		for (m = node->str; m <= p; m++)
			_putchar(*m);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		/*success*/
		return (0);
	}
	return (1); /*error*/
}

/**
 * _myalias - mimics the alias builtin
 * @inf: Struct
 * Return: Always 0
 */

int _myalias(inf_t *inf)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (inf->argc == 1) /*no additional args*/
	{
		node = inf->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	/*loop through args*/
	for (i = 1; inf->argv[i]; i++)
	{
		p = strchr(inf->argv[i], '=');
		if (p != NULL)
			set_alias(inf, inf->argv[i]);
		else
			print_alias(node_starts_with(inf->alias, inf->argv[i], '='));
	}
	/*success*/
	return (0);
}

