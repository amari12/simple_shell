#include "shell.h"

/**
 * list_len - determines length of linked list
 * @h: head
 * Return: size of list
 */

size_t list_len(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_to_strings - returns an array of strings of the list->str
 * @head: head/first node
 * Return: array - str
 */

char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t i = list_len(head), j;
	char **strs;
	char *str;

	/*check if empty*/
	if (!head || !i)
		return (NULL);
	/*memory*/
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs) /*malloc fail*/
		return (NULL);

	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}
		str = strcpy(str, node->str);
		strs[i] = str;
	} /*for*/
	/*last element*/
	strs[i] = NULL;
	return (strs);
}


/**
 * print_list - prints all elements of a list_t linked list
 * @h: head
 * Return: size of list
 */

size_t print_list(const list_t *h)
{
	size_t i = 0;

	/*loop through nodes*/
	while (h)
	{
		_puts(convert_number(h->num, 10, 0));
		/*build output*/
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		/*next node*/
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * node_starts_with - returns node whose string starts with prefix
 * @node: head
 * @prefix: string to match
 * @c: next char
 * Return: matching node or null
 */

list_t *node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	/*loop trhough nodes*/
	while (node)
	{
		/*check*/
		p = starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node); /*match found*/

		node = node->next;
	}
	/*no match*/
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @head: head
 * @node: ptr
 * Return: index of node or -1
 */

ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node) /*success*/
			return (i);
		/*next node*/
		head = head->next;
		i++;
	}
	/*fail*/
	return (-1);
}

