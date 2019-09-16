#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>

int fun(const char *file, unsigned long long num){
    FILE *fl = fopen(file, "wb");
    if(fl == NULL){
        return 0;
    }
    fwrite(&num, 1, sizeof(num), fl); 
    fclose(fl);
    return 1;
}
class FF{
public:
    std::string get_str();
};

std::string FF::get_str(){
    fprintf(stderr, "in original FF::get_str()\n");
    return "FF";
} 
