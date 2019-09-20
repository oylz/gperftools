#include <stdio.h>
#include "base.h"
#include "include/mm.h"
#include <sys/ptrace.h> 

extern "C"
{
    void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
    void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));
    void __init() __attribute__((no_instrument_function));
    int __read_small_file(const char *file, char **buf) __attribute__((no_instrument_function));
}
#include <string.h>
#include <string>
#include <dlfcn.h>

pid_t get_tid(){
#if defined OS_LINUX || defined OS_MACOSX
  #warning "GetTID11111"
  #ifndef __NR_gettid
    #warning "GetTID22222"
    #ifdef OS_MACOSX
      #define __NR_gettid SYS_gettid
    #elif ! defined __i386__
      #error "Must define __NR_gettid for non-x86 platforms"
    #else
      #define __NR_gettid 224
    #endif
  #endif
  static bool lacks_gettid = false;
  if (!lacks_gettid) {
    pid_t tid = syscall(__NR_gettid);
    if (tid != -1) {
      #warning "GetTID33333"
      return tid;
    }
    lacks_gettid = true;
  }
#endif

#if defined OS_LINUX
  return getpid(); 
#elif defined OS_WINDOWS && !defined OS_CYGWIN
  return GetCurrentThreadId();
#else
  #warning "GetTID44444"
  return (pid_t)(int)pthread_self();
#endif
}


int __read_small_file(const char *file, char **buf){
    unsigned long long rr = 0;
    FILE * fl = fopen(file, "rb");
    if(fl == NULL){
        return 0;
    }
    fseek(fl, 0, SEEK_END);
    int len = ftell(fl);
    if(len <= 0){
        fclose(fl);
        return 0;
    }
    fseek(fl, 0, SEEK_SET);
    *buf = new char[len+1];
    memset(*buf, 0, len+1);
    fread(*buf, 1, len, fl);
    fclose(fl);
    return 1;
}
unsigned long long _fun_addr = 0;
unsigned long long _ff_process_addr = 0;
unsigned long long _process_addr = 0;
unsigned long long _ff_process_addr1 = 0;
bool _has_init = false;

void __init(){
    if(_has_init){
        return;
    }
    _has_init = true;
    fprintf(stderr, "in __init\n");
    #if 1
    {
        std::string cmd = "/usr/bin/nm --demangle main | grep \"T fun(std::string const&, char, int, std::string const&\" | awk '{print $1}' > tmp.log";
        system(cmd.c_str());
        std::string file = "tmp.log";
        char *buf = NULL;
        if(0 == __read_small_file(file.c_str(), &buf)){
            fprintf(stderr, "error to find fun(int, std::string const&)\n");
            exit(0);
        }
        fprintf(stderr, "buf:%s\n", buf);
        sscanf(buf, "%lx", &_fun_addr);
        delete []buf;
    }
    {
        std::string cmd = "/usr/bin/nm --demangle main | grep \" ff::process(boost::shared_ptr<ff_node_n> const&)\" | awk '{print $1}' > tmp.log";
        system(cmd.c_str());
        std::string file = "tmp.log";
        char *buf = NULL;
        if(0 == __read_small_file(file.c_str(), &buf)){
            fprintf(stderr, "error to find ff::process(boost::shared_ptr<ff_node_n> const&\n");
            exit(0);
        }
        fprintf(stderr, "buf:%s\n", buf);
        sscanf(buf, "%lx", &_ff_process_addr);
        delete []buf;
    }
    {
        std::string cmd = "/usr/bin/nm --demangle main | grep \" process(boost::shared_ptr<ff_node_n> const&)\" | awk '{print $1}' > tmp.log";
        system(cmd.c_str());
        std::string file = "tmp.log";
        char *buf = NULL;
        if(0 == __read_small_file(file.c_str(), &buf)){
            fprintf(stderr, "error to find ff::process(boost::shared_ptr<ff_node_n> const&\n");
            exit(0);
        }
        fprintf(stderr, "buf:%s\n", buf);
        sscanf(buf, "%lx", &_process_addr);
        delete []buf;
    }
    {
        std::string cmd = "/usr/bin/nm --demangle main | grep \" ff::process(std::string const&, unsigned long, boost::shared_ptr<ff_node_n> const&, char)\" | awk '{print $1}' > tmp.log";
        system(cmd.c_str());
        std::string file = "tmp.log";
        char *buf = NULL;
        if(0 == __read_small_file(file.c_str(), &buf)){
            fprintf(stderr, "error to find ff::process(boost::shared_ptr<ff_node_n> const&\n");
            exit(0);
        }
        fprintf(stderr, "buf:%s\n", buf);
        sscanf(buf, "%lx", &_ff_process_addr1);
        delete []buf;
    }
    #else
    void *handle = dlopen("./main", RTLD_LAZY);
    if(handle == NULL){
        fprintf(stderr, "error to load main\n");
        char *err = dlerror();
        fprintf(stderr, "err:%s\n", err);
    }
    void *addr = dlsym(handle, "test");
    //void *addr = dlsym(RTLD_DEFAULT, "test");
    if(addr != NULL){
        _fun_addr = (uint64_t)addr;
        return;
    }
    char *err = dlerror();
    fprintf(stderr, "err:%s\n", err);
    #endif
}

void __cyg_profile_func_enter(void *this_fn, void *call_site){
    __init(); 
    if(this_fn == (void*)_fun_addr){
        fprintf(stderr, "\nnnnn==[fun]==beg this_fn:%lx, call_site:%lx, _fun_addr:%lx, _ff_process_addr:%lx\n", 
            this_fn, call_site, _fun_addr, _ff_process_addr);
    }
    else if(this_fn == (void*)_ff_process_addr){
        fprintf(stderr, "\nnnnn==[ff::process]==beg this_fn:%lx, call_site:%lx, _fun_addr:%lx, _ff_process_addr:%lx\n", 
            this_fn, call_site, _fun_addr, _ff_process_addr);
    }
    else if(this_fn == (void*)_process_addr){
        fprintf(stderr, "\nnnnn==[process]==beg this_fn:%lx, call_site:%lx, _process_addr:%lx\n", 
            this_fn, call_site, _process_addr);
    }
    else if(this_fn == (void*)_ff_process_addr1){
        fprintf(stderr, "\nnnnn==[ff::process 1]==beg this_fn:%lx, call_site:%lx, _ff_process_addr1:%lx\n", 
            this_fn, call_site, _ff_process_addr1);
    }
    else{
        return;
    }
    //backtrace(1);
    if(1){
        int pid = getpid();
        int tid = get_tid();
        fprintf(stderr, "pid:%d, tid:%d\n", pid, tid);
        //usleep(60*1000*1000);
        #if 0
        int child = vfork();
        if(child < 0){
            fprintf(stderr, "vfork error, exit\n");
            exit(1);
        }
        else if(child == 0){
            mm(pid, tid);
            _exit(0);
        } 
        fprintf(stderr, "now child has exit, we continue...\n");
        #else
        mm(pid, pid);
        #endif
    }
    fprintf(stderr, "enter any key to continue...\n");
    char chr = getchar();
}


void __cyg_profile_func_exit(void *this_fn, void *call_site){
    if(this_fn == (void*)_fun_addr){
        fprintf(stderr, "uuuu==[fun]==end this_fn:%lx, call_site:%lx, _fun_addr:%lx, _ff_process_addr:%lx\n", 
            this_fn, call_site, _fun_addr, _ff_process_addr);
    }
    else if(this_fn == (void*)_ff_process_addr){
        fprintf(stderr, "uuuu==[ff::process]==end this_fn:%lx, call_site:%lx, _fun_addr:%lx, _ff_process_addr:%lx\n", 
            this_fn, call_site, _fun_addr, _ff_process_addr);
    }
    else if(this_fn == (void*)_process_addr){
        fprintf(stderr, "uuuu==[process]==end this_fn:%lx, call_site:%lx, _process_addr:%lx\n", 
            this_fn, call_site, _process_addr);
    }
    else if(this_fn == (void*)_ff_process_addr1){
        fprintf(stderr, "uuuu==[ff::process 1]==end this_fn:%lx, call_site:%lx, _ff_process_addr1:%lx\n", 
            this_fn, call_site, _fun_addr, _ff_process_addr1);
    }
}

void backtrace(int to) {
    unw_cursor_t cursor;
    unw_context_t context;

    //Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    
    std::vector<trace_node> tns;

    //Unwind frames one by one, going up the frame stack.
    while (unw_step(&cursor) > 0) {
        unw_word_t offset, pc;
        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if (pc == 0) {
          break;
        }
        unw_word_t sp = 0, eh = 0, last = 0, rdi = 0, rsi = 0, rdx = 0, rcx = 0; 
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
        unw_get_reg(&cursor, UNW_REG_EH, &eh);
        unw_get_reg(&cursor, UNW_REG_LAST, &last);
        unw_get_reg(&cursor, UNW_X86_64_RDI, &rdi);
        unw_get_reg(&cursor, UNW_X86_64_RSI, &rsi);
        unw_get_reg(&cursor, UNW_X86_64_RDX, &rdx);
        unw_get_reg(&cursor, UNW_X86_64_RCX, &rcx);
        //fprintf(stderr, "\tpc:0x%lx, sp:%lx, eh:%lx, last:%lx\n", pc, sp, eh, last);
        int count = tns.size();
        if(count > 0){
            trace_node tn = tns[count-1];
            tn->size_ = sp - tn->sp_;
        }

        char sym[256];
        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
            //fprintf(stderr, "\t(%s+0x%lx)\n", sym, offset);
            trace_node tn(new trace_node_n(sym, offset, sp, pc, eh, last, 
                rdi, rsi, rdx, rcx));
            tns.push_back(tn);
        } else {
          fprintf(stderr, "\t-- error: unable to obtain symbol name for this frame\n");
        }
    }
    // print tns
    for(int i = 0; i <= to+1; i++){
        const trace_node &tn = tns[i];
        uint64_t rrdi = (tn->rdi_==0)?0:(*((uint64_t*)tn->rdi_));
        uint64_t rrsi = (tn->rsi_==0)?0:(*((uint64_t*)tn->rsi_));
        uint64_t rrdx = 0;//(tn->rdx_==0)?0:(*((uint64_t*)tn->rdx_));
        uint64_t rrcx = 0;//(tn->rcx_==0)?0:(*((uint64_t*)tn->rcx_));
        fprintf(stderr, "\tpc:0x%lx, sp:%lx, eh:%lx, last:%lx, rdi:%lx(%lx), rsi:%lx(%lx), rdx:%lx(%lx), rcx:%lx(%lx)\n", 
            tn->pc_, tn->sp_, tn->eh_, tn->last_, 
            tn->rdi_, rrdi, tn->rsi_, rrsi, tn->rdx_, rrdx, tn->rcx_, rrcx);
        fprintf(stderr, "\t(%s+0x%lx)\n", tn->sym_.c_str(), tn->offset_);
        if(i == to+1){
            break;
        } 
        {
            int count = tn->size_/8;
            uint64_t *spp = (uint64_t *)tn->sp_;
            for (int i=0; i<count; i++) {
                fprintf(stderr, "\t\t64=>*(spp + %d) = %lx\n", i, *(spp +i));
            }
        }
        #if 0
        fprintf(stderr, "\t\t------------------------------\n");
        {
            int count = tn->size_/4;
            uint32_t *spp = (uint32_t *)tn->sp_;
            for (int i=0; i<count; i++) {
                fprintf(stderr, "\t\t32=>*(spp + %d) = %lx\n", i, *(spp +i));
            }
        }   
        #endif
    }
}





