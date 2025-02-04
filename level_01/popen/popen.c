#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ft_popen(const char *file, const char *argv[], char type)
{
	int fd[2];
	pid_t pid;

	if(!file || !argv || (type != 'r' && type != 'w'))
		return -1;
	if(pipe(fd) == -1)
		return -1;
	pid = fork();
	if(pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	if(pid == 0)
	{
		if(type == 'r')
		{
			close(fd[0]);
			if(dup2(fd[1], STDOUT_FILENO) < 0)
				exit(1);
			close(fd[1]);
		}
		else if(type == 'w')
		{
			close(fd[1]);
			if(dup2(fd[0], STDIN_FILENO) < 0);
				exit(1);
			close(fd[0]);
		}
		execvp(file,(char *const *) argv);
		exit(1);
	}
	if(type == 'r')
	{
		close(fd[1]);
		return(fd[0]);
	}
	else if(type == 'w')
	{
		close(fd[0]);
		return(fd[1]);
	}
	return -1;
}
// int main()
// {
//     int fd = ft_popen("echo", (const char *[]) {"echo","hello", NULL}, 'r');
//    char buffer[1024];
//     ssize_t bytes_read;

//     // Read from the pipe
//     while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
//     {
//         buffer[bytes_read] = '\0'; // Null-terminate the buffer
//         printf("%s", buffer);
//     }

//     close(fd);
//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
#include <string.h>



int main() {
    int fd = ft_popen("cat", (const char *[]){"cat", NULL}, 'w');
    if (fd == -1) {
        perror("ft_popen");
        return 1;
    }

    // Write to the child's stdin
    const char *message = "Hello, from parent to child via pipe!\n";
    write(fd, message, strlen(message));

	 // Close the write-end to signal EOF to the child process
    close(fd); // Close the write-end to signal EOF
    return 0;
}
