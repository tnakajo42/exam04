#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe
#include <sys/wait.h> // for wait

int	picoshell(char **cmds[])
{
	int	fd[2]; // for pipe() file descriptors (fd[0] is for reading, fd[1] is for writing) 
	int	prev_fd = -1; // -1 is not a valid file descriptor
	int	status; // status of the child process
	pid_t	pid; // process id after fork() (0 for child, > 0 for parent)
	int	i; // index

	i = 0;
	// printf("%s, %s\n", *cmds[0], *cmds[1]);
	while (cmds[i] != NULL)
	{
		if (cmds[i + 1] != NULL)
		{
			if (pipe(fd) == -1) // create a pipe
				return (1);
		}
		pid = fork(); // create a child process
		if (pid < 0)
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmds[i + 1] != NULL)
			{
				close(fd[0]);
				close(fd[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1) // if there is a previous file descriptor
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1) // write to the previous file descriptor
					exit(1);
				close(prev_fd);
			}
			if (cmds[i + 1] != NULL)
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1) // read from the current file descriptor
					exit(1);
				close(fd[1]);
				close(fd[0]);
			}
			if (execvp(cmds[i][0], cmds[i]) == -1)
				exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1] != NULL)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		i++;
		
	}
	if (prev_fd != -1)
		close(prev_fd);
	while (wait(&status) > 0)
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return (1);
	return (0);
}

int	main(int ac, char **av)
{
	char	*cmd1[] = {"/bin/echo", "squalala!", NULL};
	char	*cmd2[] = {"cat", NULL};
	char	*cmd3[] = {"sed", "s/a/b/g", NULL};
	char	**cmds[] = {cmd1, cmd2, NULL};
	
	picoshell(cmds);
	return 0;
}

