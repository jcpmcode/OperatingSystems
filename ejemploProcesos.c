#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    int pid = fork();

    fork();
    fork();
    if (pid > 0) {
        printf("hello\nfather here \n");
        // wait(&spid);
    } else {
        printf("hello\nchild here \n");
        sleep(4);
    }
    return 0;
}