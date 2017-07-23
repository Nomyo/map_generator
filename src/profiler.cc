#include <profiler.hh>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <sys/vtimes.h>
#include <unistd.h>
#include <input.hh>


bool Profiler::multi_thread = false;
std::vector<double> Profiler::chunk_gen_duration;
std::vector<double> Profiler::cpu_usages;

clock_t Profiler::lastCPU, Profiler::lastSysCPU, Profiler::lastUserCPU;
int Profiler::numProcessors;

Profiler& Profiler::get_instance()
{
    static Profiler p;
    return p;
}


void Profiler::add_duration(double d)
{
    chunk_gen_duration.push_back(d);
}


void Profiler::add_cpu_usage(double d)
{
    cpu_usages.push_back(d);
}

double Profiler::get_average_duration()
{
    return 1.0 * std::accumulate(chunk_gen_duration.begin(),
				 chunk_gen_duration.end(),
				 0LL) / chunk_gen_duration.size();
}

double Profiler::get_average_usage()
{
    return 1.0 * std::accumulate(cpu_usages.begin(),
				 cpu_usages.end(),
				 0LL) / cpu_usages.size();
}

void Profiler::dump_profiling()
{
    ColorModifier clgreen(ColorCode::FG_LIGHT_GREEN);
    ColorModifier clblue(ColorCode::FG_LIGHT_BLUE);
    ColorModifier clred(ColorCode::FG_LIGHT_RED);
    ColorModifier cdef(ColorCode::FG_DEFAULT);

    auto average_duration = get_average_duration();
    auto average_usage = get_average_usage();

    std::cout << "\n\n" << std::endl;
    if (multi_thread)
	std::cout << clred << "RESOURCE USAGE SUMMARY (multi thread)" << cdef << std::endl;
    else
	std::cout << clred << "RESOURCE USAGE SUMMARY (single thread)" << cdef << std::endl;

    std::cout << clblue << "Average duration for generating chunks : " << cdef << " "
              << clgreen << average_duration << " ms" << cdef << std::endl;

    std::cout << clblue << "Average cpu usage : " << cdef << " "
              << clgreen << average_usage << " %" << cdef << std::endl;

    std::cout << "\n\n" << std::endl;
}

void Profiler::init_profiling()
{
    FILE* file;
    struct tms timeSample;
    char line[128];

    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    file = fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while(fgets(line, 128, file) != NULL)
    {
	if (strncmp(line, "processor", 9) == 0) numProcessors++;
    }

    fclose(file);
}

void Profiler::get_cpu_usage()
{
    while (true)
    {
	if (Input::terminate_)
	    return;

	struct tms timeSample;
	clock_t now;
	double percent;

	now = times(&timeSample);
	if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
	    timeSample.tms_utime < lastUserCPU)
	{
	    //Overflow detection. Just skip this value.
	    percent = -1.0;
	}
	else{
	    percent = (timeSample.tms_stime - lastSysCPU) +
		(timeSample.tms_utime - lastUserCPU);
	    percent /= (now - lastCPU);
	    percent /= numProcessors;
	    percent *= 100;
	}

	lastCPU = now;
	lastSysCPU = timeSample.tms_stime;
	lastUserCPU = timeSample.tms_utime;

	if (percent >= 0.0)
	{
	    add_cpu_usage(percent);

	    ColorModifier clgreen(ColorCode::FG_LIGHT_GREEN);
	    ColorModifier clmagenta(ColorCode::FG_LIGHT_MAGENTA);
	    ColorModifier cdef(ColorCode::FG_DEFAULT);

	    std::cout << "\n" << clmagenta << "cpu usage : "  << cdef
		      << clgreen << percent << " %" << cdef << "\n" << std::endl;

	}

	sleep(1);
    }
}
