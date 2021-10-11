#include "processor.h"
#include "linux_parser.h"

Processor::Processor()
{
    LinuxParser::CPU_Utilization util = LinuxParser::CpuUtilization();
    m_idleJiffies = util.getIdle();
    m_activeJiffies = util.getNonIdle();
}

float Processor::Utilization() 
{ 
    float CPU_Percentage{0.0f};
    LinuxParser::CPU_Utilization cpuUtil(LinuxParser::CpuUtilization());
    // get idle and non-idle params
    long idleJiffies = cpuUtil.getIdle();
    long activeJiffies = cpuUtil.getNonIdle();
    // calculate totals
    long prevTotal = m_activeJiffies + m_idleJiffies;
    long total = activeJiffies + idleJiffies;
    // calcuate difference
    long totald = total - prevTotal;
    long idled = idleJiffies - m_idleJiffies;
    // calculate percentage
    if (totald != 0)
        CPU_Percentage = (totald - idled)/static_cast<float>(totald);
    // store new cpu utilization vals
    m_idleJiffies = idleJiffies;
    m_activeJiffies = activeJiffies;
    return CPU_Percentage; 
}