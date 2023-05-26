#include "main.h"

/**
 * list_len - determines length of linked list
 * @h: pointer to first node
 *
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
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t i = list_len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * print_list - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t print_list(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_putstr(convert_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_putstr(h->str ? h->str : "(nil)");
		_putstr("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * node_starts_with - return node where str starts with prefix
 * @head: head
 * @prefix: str
 * @c: next character after prefix
 * Return: matching node / null
 */

list_t *node_starts_with(list_t *head, char *prefix, char c)
{
	char *ptr = NULL;

	/*loop until end of linked list*/
	while (head != NULL)
	{
		ptr = starts_with(head->str, prefix);
		if (ptr && ((c == -1) || (*ptr == c)))
			return (head); /*match!*/
		head = head->next; /*move to next node*/
	}
	/*no match*/
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}

