
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
    
int fun(const char *file, int num);

typedef int(*ofun)(const char *file, unsigned long long num);

int fun(const char *file, int num){
    static void *handle = NULL;
    static ofun ori_fun = NULL;

    if(!handle){
        handle = dlopen("./libfun.so", RTLD_LAZY);
        ori_fun = (ofun)dlsym(handle, "fun");
    }
    fprintf(stderr, "hack fun\n");
    return ori_fun(file, num);
}




