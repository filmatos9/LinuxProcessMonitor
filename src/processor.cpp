#include "processor.h"
#include "linux_parser.h"

#include <algorithm>

using std::string;
using std::vector;

Processor::CPU_Utilization::CPU_Utilization() : 
    user        {0l},
    nice        {0l},
    system      {0l},
    idle        {0l},
    iowait      {0l},
    irq         {0l},
    softirq     {0l},
    steal       {0l},
    guest       {0l},
    guest_nice  {0l}
{}

Processor::CPU_Utilization::CPU_Utilization(const vector<string>& strVec) 
    : Processor::CPU_Utilization()
{    
    if (strVec.size() > 9){
        user        = stol(strVec[LinuxParser::kUser_]);
        nice        = stol(strVec[LinuxParser::kNice_]);
        system      = stol(strVec[LinuxParser::kSystem_]);
        idle        = stol(strVec[LinuxParser::kIdle_]);
        iowait      = stol(strVec[LinuxParser::kIOwait_]);
        irq         = stol(strVec[LinuxParser::kIRQ_]);
        softirq     = stol(strVec[LinuxParser::kSoftIRQ_]);
        steal       = stol(strVec[LinuxParser::kSteal_]);
        guest       = stol(strVec[LinuxParser::kGuest_]);
        guest_nice  = stol(strVec[LinuxParser::kGuestNice_]);
    }
}

long Processor::CPU_Utilization::getIdle() {
    return idle + iowait;
}

long Processor::CPU_Utilization::getNonIdle() {
    return user + nice + system + irq + softirq + steal;
}

Processor::Processor() : 
    cpuUtilization_{LinuxParser::CpuUtilization()}
{}

float Processor::Utilization() 
{ 
    float CPU_Percentage{0.0f};
    CPU_Utilization newCpuUtil(LinuxParser::CpuUtilization());
    // get idle and non-idle params
    long prevIdle = cpuUtilization_.getIdle();
    long idle = newCpuUtil.getIdle();
    long prevNonIdle = cpuUtilization_.getNonIdle();
    long nonIdle = newCpuUtil.getNonIdle();
    // calculate totals
    long prevTotal = prevIdle + prevNonIdle;
    long total = idle + nonIdle;
    // calcuate difference
    long totald = total - prevTotal;
    long idled = idle - prevIdle;
    // calculate percentage
    if (totald != 0)
        CPU_Percentage = (totald - idled)/static_cast<float>(totald);
    // store new cpu utilization vals
    cpuUtilization_ = newCpuUtil;
    return CPU_Percentage; 
}