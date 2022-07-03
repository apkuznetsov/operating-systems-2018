#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int (*so_func)(int);

bool init_library(const char *so_name, const char *f_name) {
    void *hdl = dlopen(so_name, RTLD_LAZY);
    if (NULL == hdl)
        return false;

    void *p_func = dlsym(hdl, f_name);
    so_func = (typeof(so_func)) p_func;
    if (NULL == so_func)
        return false;

    return true;
}

int main(int argc, const char **argv) {
    const char *so_name = argv[1];
    const char *f_name = argv[2];
    const int f_arg = atoi(argv[3]);
    int ret_val = 0;

    if (init_library(so_name, f_name)) {
        ret_val = so_func(f_arg);
    } else {
        printf("init_library failed\n");
        return -1;
    }

    printf("%d\n", ret_val);
    return 0;
}
