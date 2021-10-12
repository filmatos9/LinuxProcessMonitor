#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
{
    m_kernel = LinuxParser::Kernel();
    m_operatingSystem = LinuxParser::OperatingSystem();
}

Processor &System::Cpu()
{
    return m_cpu;
}

vector<Process> &System::Processes()
{
    vector<int> pids(LinuxParser::Pids());
    m_processes.clear();
    m_processes.reserve(pids.size());
    std::transform(pids.begin(), pids.end(), std::back_inserter(m_processes), [](int x) { return Process(x); });

    return m_processes;
}

std::string System::Kernel() const
{
    return m_kernel;
}

float System::MemoryUtilization() const
{
    return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() const
{
    return m_operatingSystem;
}

int System::RunningProcesses() const
{
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() const
{
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() const
{
    return LinuxParser::UpTime();
}