#include <stdio.h>
#include <unistd.h> // for pid, fork, pipe, dup2, and close
#include <sys/wait.h> // for waitpid, and pid_t ??
#include <stdlib.h> // for exit

int ft_popen(const char *file, const char *argv[], char type)
{
	pid_t	pid;
	int	fd[2];
	
	if (!file || !argv || (type != 'w' && type != 'r'))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	// if fork() failed
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}// child process
	if (pid == 0)
	{
		if (type == 'r') // Reading output
		{
			if ((dup2(fd[1], STDOUT_FILENO) == -1))
				exit (-1);		    
		}
		else if (type == 'w') // Writing input
		{
			if ((dup2(fd[0], STDIN_FILENO) == -1))
				exit (-1);		    
		}
		close(fd[0]); // close read end
		close(fd[1]); // close write end
        	if (execvp(file, (char * const *)argv) == -1)
        		exit (-1);
	}
	// parent process
	if (type == 'r')
	{
		close(fd[1]); // close write end
		return (fd[0]);
	}
	else if (type == 'w')
	{
		close(fd[0]); // close read end
		return (fd[1]);
	}
	return (0);
}

int main()
{
    int fd = ft_popen("ls", (const char *[]) {"ls", NULL}, 'r');
    //int fd = ft_popen(NULL, (const char *[]) {NULL}, 'r');
    printf("fd: %i\n", fd);
    //char *line;
    //while ((line = get_next_line(fd)))
    //    ft_putstr(line);
}


