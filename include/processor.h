#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor 
{
public:
    Processor();
    float Utilization();

private:

    struct CPU_Utilization {
        CPU_Utilization();
        CPU_Utilization(const std::vector<std::string> &);
        long getIdle();
        long getNonIdle();
        long user;
        long nice;
        long system;
        long idle;
        long iowait;
        long irq;
        long softirq;
        long steal;
        long guest;
        long guest_nice;
    };
    
    CPU_Utilization cpuUtilization_;
};

#endif