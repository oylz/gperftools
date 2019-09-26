#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include "base.h"
#include <boost/thread.hpp>
#define __USE_GNU
#include <ucontext.h>


#define nprint_reg(in)\
    fprintf(stderr, "\t\tin fun self section====%s:%lx\n", #in, in);

#define NPP\
        ucontext_t uc;\
        getcontext(&uc); \
        uint64_t reg0    = uc.uc_mcontext.gregs[REG_RAX];\
        uint64_t reg1    = uc.uc_mcontext.gregs[REG_RDX];\
        uint64_t reg2    = uc.uc_mcontext.gregs[REG_RCX];\
        uint64_t reg3    = uc.uc_mcontext.gregs[REG_RBX];\
        uint64_t reg4    = uc.uc_mcontext.gregs[REG_RSI];\
        uint64_t reg5    = uc.uc_mcontext.gregs[REG_RDI];\
        uint64_t reg6    = uc.uc_mcontext.gregs[REG_RBP];\
        uint64_t reg7    = uc.uc_mcontext.gregs[REG_RSP];\
        uint64_t reg8    = uc.uc_mcontext.gregs[REG_R8];\
        uint64_t reg9    = uc.uc_mcontext.gregs[REG_R9];\
        uint64_t reg10    = uc.uc_mcontext.gregs[REG_R10];\
        uint64_t reg11    = uc.uc_mcontext.gregs[REG_R11];\
        uint64_t reg12    = uc.uc_mcontext.gregs[REG_R12];\
        uint64_t reg13    = uc.uc_mcontext.gregs[REG_R13]; \
        uint64_t reg14    = uc.uc_mcontext.gregs[REG_R14]; \
        uint64_t reg15    = uc.uc_mcontext.gregs[REG_R15]; \
        uint64_t reg16    = uc.uc_mcontext.gregs[REG_RIP];    \
        nprint_reg(reg0) \
        nprint_reg(reg1)\
        nprint_reg(reg2)\
        nprint_reg(reg3)\
        nprint_reg(reg4)\
        nprint_reg(reg5)\
        nprint_reg(reg6)\
        nprint_reg(reg7)\
        nprint_reg(reg8)\
        nprint_reg(reg9)\
        nprint_reg(reg10)\
        nprint_reg(reg11)\
        nprint_reg(reg12)\
        nprint_reg(reg13)\
        nprint_reg(reg14)\
        nprint_reg(reg15)\
        nprint_reg(reg16)

void process(const ff_node &fn){
    fprintf(stderr, "fn.addr:%lx, fn.get().addr:%lx, fn:(%s, %d)\n", &fn, fn.get(), fn->name_.c_str(), fn->age_);
}
int fun0(char xxxxchr, char mmmm){
    NPP
    #ifdef PADDR
    fprintf(stderr, "fun0 xxxxchr:%lx %x\n", &xxxxchr, xxxxchr);
    #else
    fprintf(stderr, "fun0 xxxxchr:%x, mmmm:%lx %x\n", xxxxchr, &mmmm, mmmm);
    #endif
    return 1;
}
int fun(const std::string &head, char chr, int xxxxinteger, const std::string &file, uint64_t num, char xxxxnchr){
    NPP
    #ifdef PADDR
    fprintf(stderr, "head_ptr:%lx, chr:%lx %x, xxxxinteger:%x, file_ptr:%lx, num:%lx, xxxxnchr:%lx %x\n", &head, &chr, chr, xxxxinteger, &file, num, &xxxxnchr, xxxxnchr);
    #else
    fprintf(stderr, "head_ptr:%lx, chr:%x, xxxxinteger:%x, file_ptr:%lx, num:%lx, xxxxnchr:%x\n", &head, chr, xxxxinteger, &file, num, xxxxnchr);
    #endif
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
        fprintf(stderr, "this:%lx, fn.addr:%lx, fn.get().addr:%lx, fn:(%s, %d)\n", this, &fn, fn.get(), fn->name_.c_str(), fn->age_);
    }
    void process(const std::string &str, uint64_t num, const ff_node &fn, char chr){
        fprintf(stderr, "this:%lx, str.addr:%lx, num:%lx, fn.addr:%lx, fn.get().addr:%lx, fn:(%s, %d), chr:%x\n", this, &str, num, &fn, fn.get(), fn->name_.c_str(), fn->age_, chr);
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
    int chr = 110;
    boost::thread th0(boost::bind(fun0, chr, 111));
    th0.join();
    //getchar();

    //int integer = 222222222;
    boost::thread th(boost::bind(fun, "oylzyonkenjanetjason", 110, 222222222, file, num, 111));
    th.join();
    //fun("oylzyonkenjanetjason", 110, integer, file, num, 111);
    //getchar();    
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

