#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include "base.h"

void process(const ff_node &fn){
    #if 0
    fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    backtrace(0);
    fprintf(stderr, "\tvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    #endif
    fprintf(stderr, "fnaddr:%lx, fn:(%s, %d)\n", fn.get(), fn->name_.c_str(), fn->age_);
}
int fun(const std::string &head, char chr, int integer, const std::string &file, uint64_t num, char nchr){
    #if 1
    fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    backtrace(0);
    fprintf(stderr, "\tvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    #endif
    fprintf(stderr, "head_ptr:%lx, chr:%x, integer:%x, file_ptr:%lx, num:%lx, nchr:%x\n", &head, chr, integer, &file, num, nchr);
    FILE *fl = fopen(file.c_str(), "wb");
    if(fl == NULL){
        return 0;
    }
    fwrite(&num, 1, sizeof(num), fl); 
    fclose(fl);
    return 1;
}

class ff{
public:
    void process(const ff_node &fn){
        #if 0
        fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        backtrace(0);
        fprintf(stderr, "\tvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
        #endif
        fprintf(stderr, "this:%lx, fnaddr:%lx, fn:(%s, %d)\n", this, fn.get(), fn->name_.c_str(), fn->age_);
    }
    void process(const std::string &str, uint64_t num, const ff_node &fn, char chr){
        #if 0
        fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        backtrace(0);
        fprintf(stderr, "\tvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
        #endif
        fprintf(stderr, "this:%lx, str_ptr:%lx, num:%lx, fnaddr:%lx, fn:(%s, %d), chr:%x\n", this, &str, num, fn.get(), fn->name_.c_str(), fn->age_, chr);
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
    int integer = 222222222;
    fun("oylzyonkenjanetjason", 110, integer, file, num, 111);
    
    // ff
    ff_node fn(new ff_node_n("Jason", 5));
    ff f;
    f.process(fn);

    ff_node nfn(new ff_node_n("Janet", 33));
    ff nf;
    nf.process(nfn);

    ff_node mfn(new ff_node_n("Yonken", 35));
    process(mfn);

    ff_node ofn(new ff_node_n("MM", 99));
    ff of;
    uint64_t nnum = 111;
    of.process("aa", nnum, ofn, 9);

    return 0;
}

