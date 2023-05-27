#include "shell.h"

/**
 * add_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 * Return: size of list
 */

list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_head;

	/*check head ptr*/
	if (!head)
		return (NULL);

	/*memory*/
	new_head = malloc(sizeof(list_t));
	if (!new_head) /*malloc fail*/
		return (NULL);

	/*init new node*/
	_memset((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;

	if (str) /*Check if 'str' argument is provided*/
	{
		new_head->str = strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}

	/*set up next ptr*/
	new_head->next = *head;
	*head = new_head;

	/*out*/
	return (new_head);
}

/**
 * add_node_end - adds a node to the end of the list
 * @head: head node
 * @str: str
 * @num: node index used by history
 * Return: size of list
 */

list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	/*check head ptr*/
	if (!head)
		return (NULL);

	/*Start*/
	node = *head;
	/*memory*/
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str) /*if str provided*/
	{
		new_node->str = strdup(str);
		if (!new_node->str)
		{ /*fail*/
			free(new_node);
			return (NULL);
		}
	}

	if (node)
	{
		while (node->next)
		{
			node = node->next;
		}
		node->next = new_node;
	}
	else
	{
		*head = new_node;
	}
	/*out*/
	return (new_node);
}

/**
 * print_list_str - prints only the str element of a list_t linked list
 * @h: head
 * Return: size of list
 */

size_t print_list_str(const list_t *h)
{
	size_t i = 0;

	/*loop through list*/
	while (h)
	{
		/*process*/
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		/*set up next node/loop*/
		h = h->next;
		i++;
	}

	return (i);
}

/**
 * delete_node_at_index - deletes node at given index
 * @head: head
 * @index: index of node to delete
 * Return: 1 success, 0 failure
 */

int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	/*check if empty*/
	if (!head || !*head)
		return (0);

	if (!index) /*index is 0 (deleting the head node)*/
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}

	/*setup*/
	node = *head;
	/*loop*/
	while (node)
	{
		if (i == index)
		{ /*at index - delete node*/
			prev_node->next = node->next;
			free(node->str);
			free(node);
			/*success*/
			return (1);
		}

		i++;
		prev_node = node;
		node = node->next;
	}

	/*failed to find index/delete node*/
	return (0);
}

/**
 * free_list - frees all nodes of a list
 * @head_ptr: head
 * Return: void
 */

void free_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	/*check if empty*/
	if (!head_ptr || !*head_ptr)
		return;

	/*setup*/
	head = *head_ptr;
	node = head;

	/*loop*/
	while (node)
	{
		/*rm node*/
		next_node = node->next;
		free(node->str);
		free(node);
		/*go to next node*/
		node = next_node;
	}

	/*clear head ptr*/
	*head_ptr = NULL;
}

