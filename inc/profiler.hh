#pragma once

#include <vector>
#include <color.hh>
#include <iostream>

class Profiler
{
public:
    public:
    static Profiler& get_instance();
    static void add_duration(double d);
    static void add_cpu_usage(double d);
    static void dump_profiling();
    static void init_profiling();
    static void get_cpu_usage();

private:
    Profiler() { };
    Profiler(Profiler const&) = delete;
    void operator=(const Profiler&) = delete;
    static double get_average_duration();
    static double get_average_usage();

public:
    static std::vector<double> chunk_gen_duration;
    static std::vector<double> cpu_usages;

    static clock_t lastCPU, lastSysCPU, lastUserCPU;
    static int numProcessors;
    static bool multi_thread;
};
