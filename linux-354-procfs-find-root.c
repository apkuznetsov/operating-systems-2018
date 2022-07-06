#include <stdio.h>
#include <stdlib.h> 

int pid_searching(long int pid_number) 
{
    char path[256];
    long int ppid;
    
    sprintf(path, "/proc/%ld/stat", pid_number);

    FILE *f = fopen(path, "r");
    fscanf(f, "%*d %*s %*c %ld", &ppid);
    
    return ppid;
}


int main(int argc, char * argv[]) 
{
    if (argc <= 1) {
        printf("Enter arguments!\n");
        return 1;
    }
    
    long int result_pid = atoi(argv[1]);
    
    while (result_pid != 1) {
        printf("%ld\n", result_pid);
        result_pid = pid_searching(result_pid);
    }
    
    printf("%ld\n", result_pid);
    return 0;
}
