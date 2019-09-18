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
private:
    std::string name_;
    int age_;
public:
    FF(const std::string &name, int age);
    std::string get_str();
};

std::string FF::get_str(){
    fprintf(stderr, "in original FF::get_str()\n");
    std::string rr = name_;
    rr += "#";
    rr += std::to_string(age_);
    return rr;
}
FF::FF(const std::string &name, int age){
    name_ = name;
    age_ = age;
}

