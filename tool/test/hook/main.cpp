#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>

class FF{
private:
    std::string name_;
    int age_;
public:
    FF(const std::string &name, int age);
    std::string get_str();
};
int fun(const char *file, unsigned long long num);

int main(int argc, char **argv){
    #if 0
    if(argc < 3){
        fprintf(stderr, "usage:\n./main file nums(number)\n");
        return 0;
    }
    std::string file = argv[1];
    std::string nums = argv[2];
    uint64_t num = 0;
    try{
        num = std::stoll(nums);
    }
    catch(const std::exception &e){
        fprintf(stderr, "usage:\n./main file nums(number)\n");
        return 0;
    }
    fun(file.c_str(), num);
    #endif
    //
    FF ff("Janet", 33);
    std::string str = ff.get_str(); 
    fprintf(stderr, "success!\n");
    return 0;
}

