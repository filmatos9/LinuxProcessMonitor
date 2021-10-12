#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "processor.h"

class System
{
  public:
    System();
    Processor &Cpu();
    std::vector<Process> &Processes();
    float MemoryUtilization() const;
    long UpTime() const;
    int TotalProcesses() const;
    int RunningProcesses() const;
    std::string Kernel() const;
    std::string OperatingSystem() const;

  private:
    Processor m_cpu;
    std::map<int, Process> m_processes;
    std::string m_kernel;
    std::string m_operatingSystem;
};

#endif