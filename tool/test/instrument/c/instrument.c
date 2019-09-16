#include <stdio.h>


void __attribute__((__no_instrument_function__)) __cyg_profile_func_enter(void *this_fn, void *call_site){
    fprintf(stderr, "nnnn====beg this_fn:%LX, call_site:%LX\n", this_fn, call_site);
}


void __attribute__((__no_instrument_function__)) __cyg_profile_func_exit(void *this_fn, void *call_site){
    fprintf(stderr, "uuuu====end this_fn:%LX, call_site:%LX\n", this_fn, call_site);
}
