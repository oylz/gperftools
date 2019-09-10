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
        * prof_handler:
            * call GetPC\(\)
            * call GetStackTraceWithContext\(\)
            * call ProfileData::Add\(\)


# 3.[ProfileData](./src/profiledata.cc)





