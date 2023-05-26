#include "shell.h"

/**
 * get_history_file - gets the history file
 * @inf: struct
 * Return: string -> history file
 */

char *get_history_file(inf_t *inf)
{
	char *buf, *dir;

	/*environment variable "HOME"*/
	dir = _getenv(inf, "HOME=");
	if (dir == NULL) /*(environment variable not found)*/
		return (NULL);
	/*memory*/
	buf = malloc(sizeof(char) * (strlen(dir) + strlen(HIST_FILE) + 2));
	/*malloc failed*/
	if (buf == NULL)
		return (NULL);

	buf[0] = 0; /*initialize buf as an empty string*/
	strcpy(buf, dir); /*Copy the content of dir to buf*/
	strcat(buf, "/");
	strcat(buf, HIST_FILE);

	return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @inf: struct
 * Return: 1 success, -1 ERROR
 */

int write_history(inf_t *inf)
{
	ssize_t fd;
	char *filename = get_history_file(inf);
	list_t *node = NULL;

	/*check for empty filename*/
	if (filename == NULL)
		return (-1); /*error*/
	/*open file*/
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename); /*memory*/
	/*open file failed*/
	if (fd == -1)
		return (-1);

	for (node = inf->history; node; node = node->next)
	{ /*loop through history list*/
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}

	_putfd(FLUSH, fd); /*flush*/
	close(fd); /*close hist file*/
	return (1); /*success*/
}

/**
 * read_history - reads history from file
 * @inf: struct
 * Return: histcount at success, 0 error
 */

int read_history(inf_t *inf)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(inf);

	if (filename == NULL) /*empty*/
		return (0);
	fd = open(filename, O_RDONLY); /*open file*/
	free(filename);
	if (fd == -1) /*open file failed*/
		return (0);
	if (!fstat(fd, &st)) /*get file info*/
		fsize = st.st_size;
	if (fsize < 2) /*file size less than 2*/
		return (0);

	buf = malloc(sizeof(char) * (fsize + 1)); /*memory*/
	if (buf == NULL) /*malloc fail*/
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0) /*read failed*/
		return (free(buf), 0);
	close(fd);

	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0; /*null terminate*/
			build_history_list(inf, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i) /*last line not at end of buffer*/
		build_history_list(inf, buf + last, linecount++);
	free(buf);
	inf->histcount = linecount;
	while (inf->histcount-- >= HIST_MAX) /*history count > max allowed*/
		delete_node_at_index(&(inf->history), 0);
	renumber_history(inf);
	return (inf->histcount); /*success*/
}

/**
 * build_history_list - adds entry to a history linked list
 * @inf: Struct
 * @buf: buffer
 * @linecount: the history linecount/histcount
 * Return: Always 0
 */

int build_history_list(inf_t *inf, char *buf, int linecount)
{
	list_t *node = NULL;

	/*check the history list already exists*/
	if (inf->history)
		node = inf->history;

	add_node_end(&node, buf, linecount);

	/*the history list was empty*/
	if (!inf->history)
		inf->history = node;

	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @inf: Struct
 * Return: new histcount
 */

int renumber_history(inf_t *inf)
{
	list_t *node = inf->history;
	int i = 0;

	/*iterate through nodes*/
	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	/*return updated hist count*/
	return (inf->histcount = i);
}

