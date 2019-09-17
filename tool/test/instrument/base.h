#ifndef _BASEH_
#define _BASEH_
#include <boost/shared_ptr.hpp>
#define UNW_LOCAL_ONLY
#include <libunwind.h>


struct ff_node_n{
    std::string name_;
    int age_;
    ff_node_n(const std::string &name, int age){
        name_ = name;
        age_ = age;
    }
};
typedef boost::shared_ptr<ff_node_n> ff_node;

struct trace_node_n{
    std::string sym_; 
    unw_word_t offset_;
    unw_word_t pc_;
    unw_word_t eh_;
    unw_word_t last_;
    unw_word_t sp_;
    uint64_t size_;
    unw_word_t rdi_;
    unw_word_t rsi_;
    unw_word_t rdx_;
    unw_word_t rcx_;
    trace_node_n(const std::string &sym, unw_word_t offset, unw_word_t sp, unw_word_t pc, 
        unw_word_t eh, unw_word_t last, 
        unw_word_t rdi, unw_word_t rsi, unw_word_t rdx, unw_word_t rcx){
        sym_ = sym;
        offset_ = offset;
        sp_ = sp;
        pc_ = pc;
        eh_ = eh;
        last_ = last;
        size_ = 0;
        rdi_ = rdi;
        rsi_ = rsi;
        rdx_ = rdx;
        rcx_ = rcx;
    }
};
typedef boost::shared_ptr<trace_node_n> trace_node;

extern "C"{
    void backtrace(int to) __attribute__((no_instrument_function));
}

#endif



