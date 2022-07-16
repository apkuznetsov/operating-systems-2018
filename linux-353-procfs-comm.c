#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char * argv[])
{   
    DIR *d = opendir("/proc");

    struct dirent *entry;
    char path[256];
    char pname[256];
    FILE *f;

    int count = 0;

    while ((entry = readdir(d)) != NULL) {
        if (!isdigit(*entry -> d_name))
            continue;
        
        sprintf(path, "/proc/%s/comm", entry -> d_name);
        f = fopen(path, "r");
        fgets(pname, 7, f);
        if (strcmp(pname, "genenv") == 0) {
            count++;
        }
    }

    printf("%i\n", count);

    fclose(f);
    closedir(d);

    return 0;
}
