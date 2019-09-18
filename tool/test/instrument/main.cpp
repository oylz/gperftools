#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include "base.h"
#define nbacktrace(to) \
    unw_cursor_t cursor;\
    unw_context_t context;\
\
    unw_getcontext(&context);\
    unw_init_local(&cursor, &context);\
\
    \
    std::vector<trace_node> tns;\
\
    while (unw_step(&cursor) > 0) {\
        unw_word_t offset, pc;\
        unw_get_reg(&cursor, UNW_REG_IP, &pc);\
        if (pc == 0) {\
          break;\
        }\
        unw_word_t sp = 0, eh = 0, last = 0, rdi = 0, rsi = 0, rdx = 0, rcx = 0; \
        unw_get_reg(&cursor, UNW_REG_SP, &sp);\
        unw_get_reg(&cursor, UNW_REG_EH, &eh);\
        unw_get_reg(&cursor, UNW_REG_LAST, &last);\
        unw_get_reg(&cursor, UNW_X86_64_RDI, &rdi);\
        unw_get_reg(&cursor, UNW_X86_64_RSI, &rsi);\
        unw_get_reg(&cursor, UNW_X86_64_RDX, &rdx);\
        unw_get_reg(&cursor, UNW_X86_64_RCX, &rcx);\
        int count = tns.size();\
        if(count > 0){\
            trace_node tn = tns[count-1];\
            tn->size_ = sp - tn->sp_;\
        }\
\
        char sym[256];\
        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {\
            trace_node tn(new trace_node_n(sym, offset, sp, pc, eh, last, \
                rdi, rsi, rdx, rcx));\
            tns.push_back(tn);\
        } else {\
          fprintf(stderr, "\t-- error: unable to obtain symbol name for this frame\n");\
        }\
    }\
    for(int i = to; i <= to+1; i++){\
        const trace_node &tn = tns[i];\
        uint64_t rrdi = (tn->rdi_==0)?0:(*((uint64_t*)tn->rdi_));\
        uint64_t rrsi = (tn->rsi_==0)?0:(*((uint64_t*)tn->rsi_));\
        uint64_t rrdx = 0;\
        uint64_t rrcx = 0;\
        fprintf(stderr, "\tpc:0x%lx, sp:%lx, eh:%lx, last:%lx, rdi:%lx(%lx), rsi:%lx(%lx), rdx:%lx(%lx), rcx:%lx(%lx)\n", \
            tn->pc_, tn->sp_, tn->eh_, tn->last_, \
            tn->rdi_, rrdi, tn->rsi_, rrsi, tn->rdx_, rrdx, tn->rcx_, rrcx);\
        fprintf(stderr, "\t(%s+0x%lx)\n", tn->sym_.c_str(), tn->offset_);\
        if(i == to+1){\
            break;\
        } \
        {\
            int count = tn->size_/8;\
            uint64_t *spp = (uint64_t *)tn->sp_;\
            for (int i=0; i<count; i++) {\
                fprintf(stderr, "\t\t64=>*(spp + %d) = %lx\n", i, *(spp +i));\
            }\
        }\
    }

void process(const ff_node &fn){
    #if 0
    fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    backtrace(0);
    fprintf(stderr, "\tvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
    #endif
    fprintf(stderr, "fn.addr:%lx, fn.get().addr:%lx, fn:(%s, %d)\n", &fn, fn.get(), fn->name_.c_str(), fn->age_);
}
int fun(const std::string &head, char chr, int integer, const std::string &file, uint64_t num, char nchr){
    #if 0
    fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    nbacktrace(0);
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
        fprintf(stderr, "this:%lx, fn.addr:%lx, fn.get().addr:%lx, fn:(%s, %d)\n", this, &fn, fn.get(), fn->name_.c_str(), fn->age_);
    }
    void process(const std::string &str, uint64_t num, const ff_node &fn, char chr){
        #if 0
        fprintf(stderr, "\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        backtrace(0);
        fprintf(stderr, "\tvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
        #endif
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

