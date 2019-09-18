
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <string>


typedef int(*ofun)(const char *file, unsigned long long num);

int fun(const char *file, unsigned long long num){
    static void *handle = NULL;
    static ofun ori_fun = NULL;

    if(!handle){
        #if 0
        handle = dlopen("./libfun.so", RTLD_LAZY);
        if(handle == NULL){
            fprintf(stderr, "error to load libfun.so\n");
        }
        //ori_fun = (ofun)dlsym(handle, "fun");
        ori_fun = (ofun)dlsym(handle, "_Z3funPKcy");
        #else
        ori_fun = (ofun)dlsym(RTLD_NEXT, "_Z3funPKcy");
        #endif
        if(ori_fun == NULL){
            fprintf(stderr, "error to load fun\n");
        }
    }
    fprintf(stderr, "hack fun\n");
    return ori_fun(file, num);
}
class FF{
   typedef std::string (*get_str_fun)();
    get_str_fun get_str_fun_ori_;
public:
    std::string get_str();
};

std::string FF::get_str(){
    if(get_str_fun_ori_ == NULL){
        void *ori = dlsym(RTLD_NEXT, "_ZN2FF7get_strEv"); 
        if(ori == NULL){
            fprintf(stderr, "ori is null");
            return "empty";
        }
        get_str_fun_ori_ = (get_str_fun)ori;
    }
    fprintf(stderr, "in hook FF::get_str(), get_str_fun_ori_:%lx\n", get_str_fun_ori_);
    return get_str_fun_ori_(); 
    //return "hook";
} 


