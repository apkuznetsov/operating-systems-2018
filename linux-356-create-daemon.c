#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int daemonize()
{
    int pid;
    
    if (getppid() == 1) return 0;
    
    pid = fork();

    if (pid < 0) exit(1);
    else if (pid != 0) exit(0);

    printf ("%d\n", (pid_t) getpid ());

    setsid();
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 0;
}


int main(int argc, char * argv[]) 
{
    daemonize();
    while (1) sleep(600);
}
