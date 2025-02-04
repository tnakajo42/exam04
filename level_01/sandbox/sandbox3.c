#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid = fork(); // Create a child process

    if (pid < 0)
        return -1; // Fork failed

    if (pid == 0)
    {
        // Child process: Run the function
        alarm(timeout);
        f();
        exit(0); // Exit normally if function completes
    }

    // Parent process: Monitor child with timeout
    int status;
    pid_t result;
    unsigned int elapsed = 0;

    while ((result = waitpid(pid, &status, WNOHANG)) == 0)
    {
        sleep(1); // Sleep in 1-second increments
        elapsed++;

        if (elapsed >= timeout)
        {
            kill(pid, SIGKILL); // Force kill if timeout occurs
            waitpid(pid, &status, 0); // Clean up zombie process
            if (verbose)
                printf("Bad function: timed out after %u seconds\n", timeout);
            return 0;
        }
    }

    if (result == -1)
        return -1; // waitpid failed

    // Check if child exited normally
    if (WIFEXITED(status))
    {
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

    // Check if child was terminated by a signal
    if (WIFSIGNALED(status))
    {
        int signal = WTERMSIG(status);
        if (verbose)
            printf("Bad function: %s\n", strsignal(signal));
        return 0;
    }

    return 0;
}

// Test functions
void nice_function(void)
{
    printf("I am a nice function!\n");
}

void bad_function(void)
{
    printf("I will segfault now.\n");
    int *p = NULL;
    *p = 42; // Segfault here
}

void slow_function(void)
{
    printf("I ");
    sleep(1);
    printf("am ");
    sleep(3);
    printf("a slow ");
    sleep(5);
    printf("function ");
    sleep(10);
    printf("...\n");
    sleep(100);
}

int main()
{
    sandbox(nice_function, 2, true);  // Expected: "Nice function!"
    sandbox(bad_function, 2, true);   // Expected: "Bad function: Segmentation fault"
    sandbox(slow_function, 2, true);  // Expected: "Bad function: timed out after 2 seconds"
    return 0;
}
