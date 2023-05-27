#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @inf: Struct
 * Return: str array
 */

char **get_environ(inf_t *inf)
{
	/*heck if the environment variables have been retrieved*/
	/*	or if they have changed*/
	if (!inf->environ || inf->env_changed)
	{
		inf->environ = list_to_strings(inf->env);
		inf->env_changed = 0;
	}

	/*out: array of environment variables*/
	return (inf->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @inf: Struct
 * @var: env variable
 * Return: 1 on delete, 0
 */

int _unsetenv(inf_t *inf, char *var)
{
	list_t *node = inf->env;
	size_t i = 0;
	char *p;

	/*checks:*/
	if (!node || !var)
		return (0);

	/*loop through list*/
	while (node)
	{
		/*Check if the env var starts with given name*/
		p = starts_with(node->str, var);

		if (p && *p == '=')
		{
			inf->env_changed = delete_node_at_index(&(inf->env), i);
			i = 0;
			node = inf->env;
			continue;
		}

		node = node->next; /*next node*/
		i++;
	}

	return (inf->env_changed);
}

/**
 * _setenv - Initialize a new environment variable
 * @inf: Struct
 * @var: var property
 * @value: env var value
 * Return: Always 0
 */

int _setenv(inf_t *inf, char *var, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *p;

	/*check if empty*/
	if (!var || !value)
		return (0);

	/*memory*/
	buffer = malloc(strlen(var) + strlen(value) + 2);
	if (buffer == NULL)
		return (1);

	/*prep str*/
	strcpy(buffer, var);
	strcat(buffer, "=");
	strcat(buffer, value);
	node = inf->env;
	/*loop*/
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{ /*Free the existing string*/
			free(node->str);
			node->str = buffer;
			inf->env_changed = 1;
			return (0);
		}
		node = node->next; /*next node*/
	}
	/*if no match*/
	add_node_end(&(inf->env), buffer, 0);
	free(buffer);
	inf->env_changed = 1; /*mark env var as changed*/

	return (0);
}

