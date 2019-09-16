
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int fun(char *file, int num){
    FILE *fl = fopen(file, "wb");
    if(fl == NULL){
        return 0;
    }
    fwrite(&num, 1, sizeof(num), fl); 
    fclose(fl);
    return 1;
}




