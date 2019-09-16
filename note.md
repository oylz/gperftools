# 0.cpu profile format
![cpu profile format](./docs/cpu_profiler_file_format.jpg)


# 1.[EntryPoint](./src/profiler.cc)

* user define signal [CPUPROFILESIGNAL](./src/profiler.cc#L202TL206)

* its callback function is [CpuProfilerSwitch](./src/profiler.cc#L145)
    * call ProfilerStart\(\): call CpuProfiler::Start\(\)
    * call ProfilerStop\(\): call CpuProfiler::Stop\(\)

# 2.[CpuProfiler](./src/profiler.cc)

* only define&implement class CpuProfiler in the cpp file\-\- ./src/profiler.cc

* important members:
    * ProfileData   collector_;
    * ProfileHandlerToken\* prof_handler_token_;

* important funcs:
    * Start\(\): call ProfileData::Start\(\)
    * Stop\(\): call ProfileData::Stop\(\)
    * FlushTable\(\): call ProfileData::FlushTable\(\)
    * EnableHandler\(\): call ProfileHandlerRegisterCallback\(prof_handler, this\);
        * [prof_handler](./src/profiler.cc#L336TL377):
            * call [GetPC\(\)](./src/getpc.h#L138TL156): get the instruction that was beging executed 
            * call [GetStackTraceWithContext\(\)](./src/stacktrace.cc#L298)
            * call ProfileData::Add\(\)


# 3.[ProfileData](./src/profiledata.cc)


# 4.[pprof](./src/pprof)

* a perl script file

* [ReadCPUProfile](./src/pprof#L3964)

* [subtract address](./src/pprof#L4014TL4021)
```
      printf("pc:%016x\n", $pc);
      # Subtract one from caller pc so we map back to call instr.
      # However, don't do this if we're reading a symbolized profile
      # file, in which case the subtract-one was done when the file
      # was written.
      if ($j > 0 && !$main::use_symbolized_profile) {
        $pc--;
      }
```

* [ParseLibraries](./src/pprof#L4506)


# 5.stack frame

* [stacktrace_libunwind-inl](./src/stacktrace_libunwind-inl.h#L69TL152): **GET_STACK_TRACE_OR_FRAMES**
```
 69 // Note: this part of the file is included several times.
 70 // Do not put globals below.
 71
 72 // The following 4 functions are generated from the code below:
 73 //   GetStack{Trace,Frames}()
 74 //   GetStack{Trace,Frames}WithContext()
 75 //
 76 // These functions take the following args:
 77 //   void** result: the stack-trace, as an array
 78 //   int* sizes: the size of each stack frame, as an array
 79 //               (GetStackFrames* only)
 80 //   int max_depth: the size of the result (and sizes) array(s)
 81 //   int skip_count: how many stack pointers to skip before storing in result
 82 //   void* ucp: a ucontext_t* (GetStack{Trace,Frames}WithContext only)
 83 static int GET_STACK_TRACE_OR_FRAMES {
 84   void *ip;
 85   int n = 0;
 86   unw_cursor_t cursor;
 87   unw_context_t uc;
 88 #if IS_STACK_FRAMES
 89   unw_word_t sp = 0, next_sp = 0;
 90 #endif
 91
 92   if (recursive) {
 93     return 0;
 94   }
 95   ++recursive;
 96
 97 #if (IS_WITH_CONTEXT && defined(BASE_STACKTRACE_UNW_CONTEXT_IS_UCONTEXT))
 98   if (ucp) {
 99     uc = *(static_cast<unw_context_t *>(const_cast<void *>(ucp)));
100     /* this is a bit weird. profiler.cc calls us with signal's ucontext
101      * yet passing us 2 as skip_count and essentially assuming we won't
102      * use ucontext. */
103     /* In order to fix that I'm going to assume that if ucp is
104      * non-null we're asked to ignore skip_count in case we're
105      * able to use ucp */
106     skip_count = 0;
107   } else {
108     unw_getcontext(&uc);
109     skip_count += 2;         // Do not include current and parent frame
110   }
111 #else
112   unw_getcontext(&uc);
113   skip_count += 2;         // Do not include current and parent frame
114 #endif
115
116   int ret = unw_init_local(&cursor, &uc);
117   assert(ret >= 0);
118
119   while (skip_count--) {
120     if (unw_step(&cursor) <= 0) {
121       goto out;
122     }
123 #if IS_STACK_FRAMES
124     if (unw_get_reg(&cursor, UNW_REG_SP, &next_sp)) {
125       goto out;
126     }
127 #endif
128   }
129
130   while (n < max_depth) {
131     if (unw_get_reg(&cursor, UNW_REG_IP, (unw_word_t *) &ip) < 0) {
132       break;
133     }
134 #if IS_STACK_FRAMES
135     sizes[n] = 0;
136 #endif
137     result[n++] = ip;
138     if (unw_step(&cursor) <= 0) {
139       break;
140     }
141 #if IS_STACK_FRAMES
142     sp = next_sp;
143     if (unw_get_reg(&cursor, UNW_REG_SP, &next_sp) , 0) {
144       break;
145     }
146     sizes[n - 1] = next_sp - sp;
147 #endif
148   }
149 out:
150   --recursive;
151   return n;
152 }
```

* [setup_inl_a](./src/stacktrace_impl_setup-inl.h#L75TL80): **STACKTRACE_INL_HEADER**
```
 75 #define IS_STACK_FRAMES 1
 76 #define IS_WITH_CONTEXT 1
 77 #define GET_STACK_TRACE_OR_FRAMES \
 78   SIS_CONCAT(GetStackFramesWithContext_, GST_SUFFIX)(void **result, int *sizes, int max_depth, \
 79                                                     int skip_count, const void *ucp)
 80 #include STACKTRACE_INL_HEADER
```

* [setup_inl_b](./src/stacktrace_impl_setup-inl.h#L85TL91)
```
 85 static GetStackImplementation SIS_CONCAT(impl__,GST_SUFFIX) = {
 86   SIS_CONCAT(GetStackFrames_, GST_SUFFIX),
 87   SIS_CONCAT(GetStackFramesWithContext_, GST_SUFFIX),
 88   SIS_CONCAT(GetStackTrace_, GST_SUFFIX),
 89   SIS_CONCAT(GetStackTraceWithContext_, GST_SUFFIX),
 90   SIS_STRINGIFY(GST_SUFFIX)
 91 };
```

* [stacktrace-a](./src/stacktrace.cc#L105TL115): **define STACKTRACE_INL_HEADER "stacktrace_libunwind-inl.h"**
```
105 // libunwind uses __thread so we check for both libunwind.h and
106 // __thread support
107 #if defined(HAVE_LIBUNWIND_H) && defined(HAVE_TLS)
108 #define STACKTRACE_INL_HEADER "stacktrace_libunwind-inl.h"
109 #define GST_SUFFIX libunwind
110 #warning "gst_suffix====libunwind"
111 #include "stacktrace_impl_setup-inl.h"
112 #undef GST_SUFFIX
113 #undef STACKTRACE_INL_HEADER
114 #define HAVE_GST_libunwind
115 #endif // HAVE_LIBUNWIND_H
```

* [stacktrace-b](./src/stacktrace.cc#L223TL225)
```
223 #elif defined(HAVE_GST_libunwind)
224 #warning "stack_impl=========libunwind"
225 static GetStackImplementation *get_stack_impl = &impl__libunwind;
```



















