#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

/**
"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal_description>\n"
"Bad function: timed out after <timeout> seconds\n"
 */

int sandbox(void(*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int	status;
	unsigned int	time = 0;
	int	exit_code;
	int	signal;

	pid = fork();
	if (pid < 0)
		return -1;
	// Child process
	if (pid == 0)
	{
		alarm(timeout); // Set an alarm to kill the process after timeout seconds (exit_code is exested)  
		f();
		exit(0);
	}
	// About "WNOHANG" in waitpid function?
	// WNOHANG is defined as 1.
	// W = Wait
	// NOHANG = No Hanging (no stopping the program)
	while (waitpid(pid, &status, WNOHANG) == 0)
	{
		sleep(1);
		time++;
		if (time >= timeout)
		{
			kill(pid, SIGKILL);
			waitpid(pid, &status, 0); // You need to wait for the process to finish
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return 0;
		}
	}
	// WIFEXITED is "Wait IF the process EXITED normally."
	if (WIFEXITED(status))
	{
		// "Wait EXIT STATUS" and is used to get
		// the exit code of a child process after it finishes.
		exit_code = WEXITSTATUS(status);
		if (exit_code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return 1;
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %i\n", exit_code);
			return 0;				
		}
	}
	// Wait ... IF SIGNALED?
	if (WIFSIGNALED(status))
	{
		// Wait TERMinated SIGnal
		signal = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %s\n", strsignal(signal));
		return 0;
	}	
	return 0;
}

void	bad_timeout()
{
	printf("hi ...\n");
	sleep(5);
	printf("oh you are still here\n");
	sleep(100000);
}

void	bad_exit()
{
	printf("I will exit with code 42...\n");
	exit(42); // Explicitly exit with a non-zero code
}

void	bad_func()
{
	printf("you will segfault...\n");
	int	*p = NULL;
	*p = 42;
}

void	nice_func()
{
	printf(">>>You are nice<<<\n");
}

int	main(void)
{
	// sandbox(nice_func, 10, true);
	// sandbox(bad_timeout, 10, true);
	// sandbox(bad_func, 10, true);
	sandbox(bad_exit, 10, true);
	return (0);
}
