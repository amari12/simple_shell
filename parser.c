#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @inf: struct
 * @path: path to file
 * Return: 1 true, 0 false
 */

int is_cmd(inf_t *inf, char *path)
{
	struct stat st;

	/*unused par*/
	(void)inf;

	/*check*/
	if (path == NULL || stat(path, &st))
		return (0);
	if (st.st_mode & S_IFREG)
	{
		return (1); /*success*/
	}
	/*default*/
	return (0);
}

/**
 * dup_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: start index
 * @stop: stop index
 * Return: ptr to new buffer
 */

char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	/*loop throug*/
	for (k = 0, i = start; i < stop; i++)
	{
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	}

	/*nul terminator*/
	buf[k] = 0;
	/*out*/
	return (buf);
}

/**
 * find_path - finds this cmd in the PATH string
 * @inf: struct
 * @pathstr: the PATH
 * @cmd: cmd
 * Return: full path of cmd if found || NULL
 */

char *find_path(inf_t *inf, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (pathstr == NULL) /*empty*/
		return (NULL);

	if ((strlen(cmd) > 2) && starts_with(cmd, "./"))
	{ /*If the command starts with "./"*/
		if (is_cmd(inf, cmd)) /*valid*/
			return (cmd);
	}

	while (1) /*loop*/
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, curr_pos, i);
			/*check:*/
			if (!*path) /*empty*/
				strcat(path, cmd);
			else
			{
				strcat(path, "/");
				strcat(path, cmd);
			}
			if (is_cmd(inf, path)) /*If constructed path= valid executable*/
				return (path);
			if (!pathstr[i]) /*end of path string*/
				break; /*exit loop*/
			curr_pos = i;
		}
		i++;
	}
	/*default*/
	return (NULL);
}

