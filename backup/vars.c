#include "main.h"

/**
 * is_chain - checks if char in buffer is a chain delimeter
 * @inf: args
 * @buffer: string
 * @p: current position
 * Return: int (1 if chain delimeter, 0 )
 */

int is_chain(inf_t *inf, char *buffer, size_t *p)
{
	size_t j = *p;

	/*Check if the current position in the buffer contains '||'(CMD_OR)*/
	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		inf->cmd_buffer_type = CMD_OR;
	}
	/*Check if the current position in the buffer contains '&&'(CMD_AND)*/
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		inf->cmd_buffer_type = CMD_AND;
	}
	/*';' (CMD_CHAIN)*/
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		inf->cmd_buffer_type = CMD_CHAIN;
	}
	else
		return (0); /*no cmd chain found*/
	/*updtae pointer*/
	*p = j;
	return (1); /*cmd chain found*/
}

/**
 * check_chain - checks if proceed chaining based on last status
 * @inf: args
 * @buffer: string
 * @p: current position
 * @i: starting position
 * @len: length of buffer
 * Return: Void
 */

void check_chain(inf_t *inf, char *buffer, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	/*check if cmd chain is CMD_AND*/
	if (inf->cmd_buffer_type == CMD_AND)
	{
		if (inf->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}
	/*check if cmd chain is CMD_OR*/
	if (inf->cmd_buffer_type == CMD_OR)
	{
		if (!inf->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}
	/*updtae pointer*/
	*p = j;
}

/**
 * replace_alias - replace aliases in the tokenised string
 * @inf: args
 * Return: int ( 1 replaced, 0 fail)
 */

int replace_alias(inf_t *inf)
{
	int i;
	list_t *node;
	char *ptr;

	/*10 iterations*/
	for (i = 0; i < 10; i++)
	{
		/*find node in alias list*/
		node = node_starts_with(inf->alias, inf->argv[0], '=');
		if (node != NULL) /*no match*/
			return (0);
		/*free existing cmd name*/
		free(inf->argv[0]);
		/*get index of "="*/
		ptr = strchr(node->str, '=');
		if (ptr != NULL) /*"=" not found*/
			return (0);
		/* duplicate string after "="*/
		ptr = strdup(ptr + 1);
		if (ptr != NULL) /*duplication failed*/
			return (0);
		inf->argv[0] = ptr; /*assign new cmd / replace*/
	}
	return (1); /*success*/
}

/**
 * replace_vars - replaces vars in tokenised str
 * @inf: args
 * Return: int ( 1 replaced, 0 failed)
 */

int replace_vars(inf_t *inf)
{
	int i = 0;
	list_t *node;
	char *s;

	/*go through arguments*/
	for (i = 0; inf->argv[i]; i++)
	{
		/*start with $*/
		if (inf->argv[i][0] != '$' || !inf->argv[i][1])
			continue;
		/*only $*/
		if (!strcmp(inf->argv[i], "$?"))
		{
			s = strdup(convert_number(inf->status, 10, 0));
			replace_string(&(inf->argv[i]), s);
			continue;
		}
		/*$$*/
		if (!strcmp(inf->argv[i], "$$"))
		{ /*Replace the argument with the string rep of current pID*/
			s = strdup(convert_number(getpid(), 10, 0));
			replace_string(&(inf->argv[i]), s);
			continue;
		}
		/*find node*/
		node = node_starts_with(inf->env, &inf->argv[i][1], '=');
		if (node != NULL)
		{ /*replace the argument with the value of the env variable*/
			s = strdup(strchr(node->str, '=') + 1);
			replace_string(&(inf->argv[i]), s);
			continue;
		}
		s = strdup("");
		replace_string(&inf->argv[i], s);
	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: old str
 * @new: new str
 * Return: int ( 1 replaced, 0 fail)
 */

int replace_string(char **old, char *new)
{
	/*replace old str with new str*/
	free(*old);
	*old = new;
	return (1);
}

