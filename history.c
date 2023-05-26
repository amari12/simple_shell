#include "main.h"

/**
 * read_history - reads history from file
 * @inf: the parameter struct
 * Return: h_count on success, 0 error
 */

int read_history(inf_t *inf)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buffer = NULL, *filename;

	filename = get_history_file(inf);
	if (filename == NULL) /*failed to get filename*/
		return (0);
	fd = open(filename, O_RDONLY);
	free(filename); /*memory*/
	if (fd == -1) /*failed ot open file*/
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size; /*get size*/
	if (fsize < 2) /*file size less than 2 bytes*/
		return (0);
	buffer = malloc(sizeof(char) * (fsize + 1));
	if (buffer == NULL) /*malloc failed*/
		return (0);
	rdlen = read(fd, buffer, fsize);
	/*null terminate*/
	buffer[fsize] = 0;
	if (rdlen <= 0) /*read failed / nothing was read */
		return (free(buffer), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0; /*replace new line char*/
			/*add to history list*/
			build_history_list(inf, buffer + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(inf, buffer + last, linecount++);
	free(buffer); /*memory*/
	inf->h_count = linecount;
	while (inf->h_count-- >= HIST_MAX) /*delete history*/
		delete_node_at_index(&(inf->history), 0);
	renumber_history(inf);
	/*return final history count*/
	return (inf->h_count);
}

/**
 * get_history_file - gets history file
 * @inf: arguments
 * Return: string buffer -> history file
 */

char *get_history_file(inf_t *inf)
{
	char *buffer, *dir;
	size_t len, len2;

	dir = _getenv(inf, "HOME=");
	if (dir == NULL)
		return (NULL);
	len = strlen(dir);
	len2 = strlen(HIST_FILE);
	buffer = malloc(sizeof(char) * (len + len2 + 2));
	if (buffer == NULL)
		return (NULL);
	strcpy(buffer, dir);
	buffer[len] = '/';
	strcpy(buffer + len + 1, HIST_FILE);
	return (buffer);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @inf: the parameter struct
 * Return: 1 on success, else -1
 */

int write_history(inf_t *inf)
{
	ssize_t fd;
	char *filename = get_history_file(inf);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = inf->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * build_history_list - entry to history linked list
 * @inf: arguments
 * @buffer: buffer
 * @linecount: history linecount, h_count
 * Return: Always 0
 */

int build_history_list(inf_t *inf, char *buffer, int linecount)
{
	list_t *node = NULL;

	if (inf->history != NULL)
		node = inf->history;
	/*add node to linked list*/
	add_node_end(&node, buffer, linecount);
	/*history list empty?*/
	if (inf->history == NULL)
		inf->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked_list
 * @inf: arguments
 * Return: the new history count
 */

int renumber_history(inf_t *inf)
{
	list_t *node = inf->history;
	int i = 0, count;

	while (node != NULL)
	{
		node->num = i++;
		node = node->next;
	}
	count = inf->h_count;
	return (count);
}

