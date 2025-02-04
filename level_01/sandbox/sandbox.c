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
	pid_t pid;
	int status;
	int result;
	int exit_code;
	int sig;

	if(!f || timeout == 0 || timeout > 100000000)
		return -1;
	pid = fork();
	if (pid < 0)
	{
		if (verbose)
		{
			printf("Fork failed");
			printf("Error_code: %d\n", errno);
		}
		return -1;
	}
	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(0);
	}
	result = waitpid(pid, &status, 0);
	if(result < 0)
	{
		if(verbose)
		{
			printf("Waitpid failed");
			printf("Error_code :%d\n", errno);
		}
		return -1;
	}
	if(WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGALRM)
		{
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout); // Timeout message
			return 0; // Indicate a timeout
		}
		if (verbose)
			printf("Bad function: %s\n", strsignal(sig));
		return 0;
	}
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		if (exit_code !=0)
		{
			if(verbose)
				printf("Bad function : exited with code %d\n", exit_code);
			return 0;
		}
		else if (exit_code == 0)
		{
			if(verbose)
				printf("Nice function!\n");
			return 1;
		}
	}
	if (verbose)
		printf("Unknown termination\n");
	return -1;
}
void	nice_function(void)
{
	printf("I am a nice function!\n");
}

void	bad_function(void)
{
	printf("I will segfault now.\n");
	int *p = NULL;
	*p = 42; // Segfault here
}

void	slow_function(void)
{
	printf("I ");
	sleep(1); // Longer than timeout
	printf("am ");
	sleep(3); // Longer than timeout
	printf("a slow ");
	sleep(5); // Longer than timeout
	printf("function ");
	sleep(10); // Longer than timeout
	printf("...\n");
	sleep(100); // Longer than timeout
}

int	main()
{
	sandbox(nice_function, 2, true);  // Expected: "Nice function!"
	sandbox(bad_function, 2, true);  // Expected: "Bad function: Segmentation fault"
	sandbox(slow_function, 2, true); // Expected: "Bad function: timed out after 2 seconds"
	return 0;
}
