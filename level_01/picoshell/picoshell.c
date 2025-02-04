#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
	int fd[2];
	int i;
	int prev_fd;
	int pid;

	i = 0;
	prev_fd = 0;
	while(cmds[i])
	{
		if(cmds[i+1])
		{
			if (pipe(fd) == -1)
				return (1);
		}
		pid = fork();
		if(pid < 0)
			return (1);
		if(pid == 0)
		{
			if(prev_fd !=0)
			{
				if(dup2(prev_fd, STDIN_FILENO) == -1)
					return (1);
				close(prev_fd);
			}
			if(cmds[i+1])
			{
				close(fd[0]);
				if(dup2(fd[1], STDOUT_FILENO) == -1)
					return (1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if(prev_fd !=0)
				close(prev_fd);
			if(cmds[i+1])
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		i++;
	}
	while(wait(NULL) > 0)
		;
	return (0);
}

int main() {
    // Example usage: picoshell with commands
    char *cmd1[] = {"/bin/echo","hello", "\n","berlin", NULL};
    char *cmd2[] = {"/usr/bin/wc", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};

    picoshell(cmds);
    return 0;
}
