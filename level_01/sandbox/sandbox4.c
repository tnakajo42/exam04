#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int	sandbox(void(*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return -1;
	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(0);
	}
	
	int	status;
	pid_t	result;
	unsigned int	time = 0;
	
	// About "WNOHANG" in waitpid function?
	// W = Wait
	// NOHANG = No Hanging (no stopping the program)
	result = waitpid(pid, &status, WNOHANG);
	
	if (result == -1)
        	return -1;
       
	while (result == 0)
	{
		sleep(1); // Sleep in 1 Second increments
		time++;
		if (time >= timeout)
		{
			kill(pid, SIGKILL); // force kill if it's timeout
			waitpid(pid, &status, 0);
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return 0;
		}
		result = waitpid(pid, &status, WNOHANG);
	}
	
	
	// WIFEXITED is "Wait IF the process EXITED normally."
	if (WIFEXITED(status))
	{
		// "Wait EXIT STATUS" and is used to get
		// the exit code of a child process after it finishes.
		int exit_code = WEXITSTATUS(status);
		if (exit_code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return 1;
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", exit_code);
			return 0;
		}
	}
	
	// Wait ... IF SIGNALED?
	if (WIFSIGNALED(status))
	{
		// Wait TERMinated SIGnal
		int signal = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %s\n", strsignal(signal));
		return (0);
	}
		
	return 0;
}

void	nice_function(void)
{
	int	i = 100;
	printf("%i i'm nices\n", i);
}

int	main()
{
	sandbox(nice_function, 2, true);
	return (0);
}

