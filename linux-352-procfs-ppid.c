#include <stdio.h>

int main(int argc, char * argv[])
{
    long long int ppid;
    FILE *f = fopen("/proc/self/stat", "r");

    fscanf(f, "%*d %*s %*c %lld", &ppid);
    printf("%lld\n", ppid);

    fclose(f);
    return 0;
}