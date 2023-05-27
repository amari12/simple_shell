#include "shell.h"

/**
 * is_chain - checks if char in buffer is a chain delimeter
 * @inf: args
 * @buffer: string
 * @p: current position
 * Return: int 1 if chain delimeter, 0
 */

int is_chain(inf_t *inf, char *buffer, size_t *p)
{
	size_t j = *p;

	/*Check if the current position in the buffer contains '||'(CMD_OR)*/
	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_OR;
	}
	/*Check if the current position in the buffer contains '&&'(CMD_AND)*/
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_AND;
	}
	/*';' (CMD_CHAIN)*/
	else if (buffer[j] == ';') /*end of cmd */
	{
		buffer[j] = 0; /* replace semicolon with null */
		inf->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0); /*no cmd chain found*/
	/*no cmd chain found*/
	*p = j;
	/*cmd chain found:*/
	return (1);
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
	if (inf->cmd_buf_type == CMD_AND)
	{
		if (inf->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}

	/*check if cmd chain is CMD_OR*/
	if (inf->cmd_buf_type == CMD_OR)
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
	char *p;

	/*10 iterations*/
	for (i = 0; i < 10; i++)
	{
		/*find node in alias list*/
		node = node_starts_with(inf->alias, inf->argv[0], '=');
		/*no match*/
		if (!node)
			return (0);
		/*free existing cmd name*/
		free(inf->argv[0]);

		p = strchr(node->str, '='); /*get index of "="*/
		if (!p) /*"=" not found*/
			return (0);

		/* duplicate string after "="*/
		p = strdup(p + 1);
		if (p == NULL) /*duplication failed*/
			return (0);
		/*assign new cmd / replace*/
		inf->argv[0] = p;
	}
	/*success*/
	return (1);
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

	/*go through arguments*/
	for (i = 0; inf->argv[i]; i++)
	{
		/*start with $*/
		if (inf->argv[i][0] != '$' || !inf->argv[i][1])
			continue;
		/*only $*/
		if (!strcmp(inf->argv[i], "$?"))
		{ /*Replace the argument with the string rep of current pID*/
			replace_string(&(inf->argv[i]),
				strdup(convert_number(inf->status, 10, 0)));
			continue;
		}
		/*$$*/
		if (!strcmp(inf->argv[i], "$$"))
		{
			replace_string(&(inf->argv[i]),
				strdup(convert_number(getpid(), 10, 0)));
			continue;
		}

		node = node_starts_with(inf->env, &inf->argv[i][1], '=');
		if (node) /*replace the argument with the value of the env variable*/
		{
			replace_string(&(inf->argv[i]),
				strdup(strchr(node->str, '=') + 1));
			continue;
		}

		replace_string(&inf->argv[i], strdup(""));
	}
	/*out*/
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

