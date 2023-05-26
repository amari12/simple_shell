#include "shell.h"

/**
 * _myenv - prints current environment
 * @inf: Struct
 * Return: Always 0
 */

int _myenv(inf_t *inf)
{
	/*print out env variables*/
	print_list_str(inf->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @inf: Structure
 * @name: env variable name
 * Return: env variable value
 */

char *_getenv(inf_t *inf, const char *name)
{
	list_t *node = inf->env;
	char *p;

	while (node != NULL)
	{ /*loop*/
		/*check for match*/
		p = starts_with(node->str, name);
		if (p && *p) /*both not empty*/
			return (p);
		/*if not match - next*/
		node = node->next;
	}
	/*last*/
	return (NULL);
}

/**
 * _mysetenv - Initialize a new environment variable
 * @inf: Struct
 * Return: Always 0
 */

int _mysetenv(inf_t *inf)
{
	/*args connot be 3*/
	if (inf->argc != 3)
	{ /*fail*/
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	/*set env*/
	if (_setenv(inf, inf->argv[1], inf->argv[2]))
		return (0); /*success*/
	return (1); /*fail*/
}

/**
 * _myunsetenv - Remove an environment variable
 * @inf: Struct
 * Return: Always 0
 */

int _myunsetenv(inf_t *inf)
{
	int i;

	/*check nr of args*/
	if (inf->argc == 1)
	{ /*error*/
		_eputs("Too few arguements.\n");
		return (1);
	}
	/*loop trough*/
	for (i = 1; i <= inf->argc; i++)
	{ /*un set env var*/
		_unsetenv(inf, inf->argv[i]);
	}
	/*success*/
	return (0);
}

/**
 * populate_env_list - populate env linked list
 * @inf: Struct
 * Return: Always 0
 */

int populate_env_list(inf_t *inf)
{
	list_t *node = NULL;
	size_t i;

	/*loop trhoug*/
	for (i = 0; environ[i]; i++)
	{
		/*add new node*/
		add_node_end(&node, environ[i], 0);
	}
	/*set env c=value*/
	inf->env = node;
	/*success*/
	return (0);
}

