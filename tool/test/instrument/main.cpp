#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>

int fun(const std::string &file, uint64_t num){
    FILE *fl = fopen(file.c_str(), "wb");
    if(fl == NULL){
        return 0;
    }
    fwrite(&num, 1, sizeof(num), fl); 
    fclose(fl);
    return 1;
}
std::string nfun(const std::string &file){
    std::string rr = "";
    FILE *fl = fopen(file.c_str(), "rb");
    if(fl == NULL){
        return rr;
    }
    fseek(fl, 0, SEEK_END);
    int len = ftell(fl);
    char *buf = new char[len];
    fread(buf, 1, len, fl); 
    fclose(fl);
    rr = std::string(buf, len);
    return rr;
}
class ff{
private:
    int a_;
public:
    ff(int a){
        a_ = a;
    }
    void process(){
        fprintf(stderr, "a_:%d\n", a_);
    }
};

int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "usage:\n./main file nums(number)\n");
        return 0;
    }
    std::string file = argv[1];
    std::string nums = argv[2];
    // fun
    uint64_t num = 0;
    try{
        num = std::stoll(nums);
    }
    catch(const std::exception &e){
        fprintf(stderr, "usage:\n./main file nums(number)\n");
        return 0;
    }
    fun(file, num);
    // nfun
    std::string rr = nfun(file);
    uint64_t rrn = 0;
    memcpy(&rrn, rr.data(), rr.size());
    fprintf(stderr, "rrn:%lu\n", rrn);

    // ff
    ff f(100);
    f.process();
    return 0;
}

