#include "shell.h"

/**
 * main - entry point
 * @argc: argc
 * @argv: argv
 * Return: 0 success, 1 error
 */

int main(int argc, char **argv)
{
	inf_t inf[] = { INFO_INIT };
	int fd = 2;

	/*assembly code to move the value of 'fd' into register %0*/
	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		/*couldn't open file*/
		if (fd == -1)
		{
			if (errno == EACCES) /*error bc of insufficient permissions*/
				exit(126);
			if (errno == ENOENT) /*error: file not found*/
			{
				/*assemble output*/
				_eputs(argv[0]);
				_eputs(": 0: Can't open ");
				_eputs(argv[1]);
				_eputchar('\n');
				_eputchar(FLUSH);
				exit(127); /*Exit with status 127*/
			}
			return (EXIT_FAILURE);
		}
		inf->readfd = fd;
	}
	populate_env_list(inf);
	read_history(inf);
	loop(inf, argv);

	/*success*/
	return (EXIT_SUCCESS);
}

