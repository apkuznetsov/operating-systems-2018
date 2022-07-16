#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void exit_program()
{
    exit(0);
}

int main()
{
    pid_t pid;

    switch (pid = fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            chdir("/");
            setsid();

            printf("%d\n", getpid());

            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);

            signal(SIGURG, exit_program);

            while (1) {
            }
    }

    return 0;
}
