// Author: Jose Carlos Paniagua
// Date: January 25, 2021

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Using fork to execute two commands from a single program
    int pid = fork();
    char *command0[] = {"/bin/pwd", NULL, NULL};
    char *command1[] = {"/bin/ls", "-lrt", NULL};

    if (pid > 0)
    {
        printf("Full path:\n");
        execve(command0[0], command0, NULL);
        wait(&pid);
    }
    else
    {
        sleep(4);
        printf("\nContent:\n");
        execve(command1[0], command1, NULL);
    }

    return 0;
}