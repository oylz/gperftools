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


int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "usage:\n./main file nums(number)\n");
        return 0;
    }
    char *file = argv[1];
    char *nums = argv[2];
    int num = 0;
    sscanf(nums, "%d", &num);
    fun(file, num);
    return 0;
}

