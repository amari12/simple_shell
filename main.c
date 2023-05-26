#include "main.h"

/**
* main - entry point
* @argc: arg count
* @argv: arg vector
* Return: int (0 on success, 1 on error)
*/

int main(int argc, char **argv)
{
	inf_t inf[] = { INF_INIT }; /*initialise*/
	int file_d = 2; /*file descriptor*/
	char err_msg[256];
	int err_msg_len;

	/*assembly code to move the value of 'fd' into register %0*/
	/*asm ("mov %1, %0\n\t"*/
	/*		"add $3, %0"*/
	/*		: "=r" (file_d)*/
	/*		: "r" (file_d));*/
	file_d += 3;

	if (argc == 2)
	{
		file_d = open(argv[1], O_RDONLY);
		if (file_d == -1) /*couldn't open file*/
		{
			if (errno == EACCES) /*error bc of insufficient permissions*/
				exit(126); /*exit status*/
			if (errno == ENOENT) /*error: file not found*/
			{
				err_msg_len = snprintf(err_msg, sizeof(err_msg),
						"%s: 0: Can't open %s\n", argv[0], argv[1]);
				write(STDERR_FILENO, err_msg, err_msg_len);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		inf->readfd = file_d;
	}
	populate_env_list(inf);
	read_history(inf);
	hsh(inf, argv);
	return (EXIT_SUCCESS);
}

