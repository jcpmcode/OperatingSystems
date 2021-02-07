#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    int pid = fork();

    char *argv[] = {"/bin/ls", "-lrt", NULL};

    if (pid > 0)
    {
        printf("hello\nfather here \n");
        wait(&pid);
    }
    else
    {
        printf("hello\nchild here \n");
        sleep(4);
        execve(argv[0], argv, NULL);
    }

    return 0;
}