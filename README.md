Simple shell
May 2023

Contirbutors:
	Amari Kotze (amari12)
	Ntlakala Selamolela (Ntlakala)

Github repository owners:

Simple shell process:
1. Reading User Input:
The first step is to read input from the user, We can use the "fgets" function in C to read input from the user. Once we have the input, we can parse it to determine the command and its arguments.

2. Parsing User Input:
Once we have the input from the user, we need to parse it to separate the command and its arguments. We can use the "strtok" function in C to split the input into tokens based on spaces. The first token will be the command, and the remaining tokens will be the arguments.

3. Forking a Child Process:
After parsing the user input, we need to fork a child process. We can use the "fork" function in C to create a new process that is an exact copy of the parent process.

4. Executing the Command:
Once we have forked a child process, we need to execute the command. We can use the "execvp" function in C to execute the command with its arguments. This function replaces the current process with the new process that runs the specified command.

5. Waiting for the Child Process to Finish:
After executing the command(s), we need to wait for the child process to finish. We can use the "wait" function in C to wait for the child process to finish executing.

6. Looping back to Step 1:
After the child process has finished executing, we can loop back to step 1 and read input from the user again.

