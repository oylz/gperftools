#include <stdio.h>
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
unsigned long long _addr = 0;
unsigned long long _naddr = 0;

void __init(){
    if(_addr > 0){
        return;
    }
    fprintf(stderr, "in __init\n");
    #if 1
    {
        std::string cmd = "/usr/bin/nm --demangle main | grep \"T fun(std::string const&\" | awk '{print $1}' > tmp.log";
        system(cmd.c_str());
        std::string file = "tmp.log";
        char *buf = NULL;
        if(0 == __read_small_file(file.c_str(), &buf)){
            fprintf(stderr, "error to find fun(std::string const&)\n");
            exit(0);
        }
        fprintf(stderr, "buf:%s\n", buf);
        sscanf(buf, "%LX", &_addr);
        delete []buf;
    }
    {
        std::string cmd = "/usr/bin/nm --demangle main | grep \"ff::process()\" | awk '{print $1}' > tmp.log";
        system(cmd.c_str());
        std::string file = "tmp.log";
        char *buf = NULL;
        if(0 == __read_small_file(file.c_str(), &buf)){
            fprintf(stderr, "error to find fun(std::string const&)\n");
            exit(0);
        }
        fprintf(stderr, "buf:%s\n", buf);
        sscanf(buf, "%LX", &_naddr);
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
        _addr = (uint64_t)addr;
        return;
    }
    char *err = dlerror();
    fprintf(stderr, "err:%s\n", err);
    #endif
}

void __cyg_profile_func_enter(void *this_fn, void *call_site){
    __init(); 
    fprintf(stderr, "nnnn====beg this_fn:%LX, call_site:%LX, fun's _addr:%LX, ff::process()'s _naddr:%LX\n", 
        this_fn, call_site, _addr, _naddr);
}


void __cyg_profile_func_exit(void *this_fn, void *call_site){
    fprintf(stderr, "uuuu====end this_fn:%LX, call_site:%LX\n", this_fn, call_site);
}






