#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int counter1 = 0;
int counter2 = 0;

void increment_counter1()
{
    counter1 += 1;
}

void increment_counter2()
{
    counter2 += 1;
}

void exit_program()
{
    printf("%d %d\n", counter1, counter2);
    exit(0);
}

int main()
{
    signal(SIGUSR1, increment_counter1);
    signal(SIGUSR2, increment_counter2);
    signal(SIGTERM, exit_program);

    while (1) {}
}
