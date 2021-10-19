#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <map>
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
    // get all active pids
    vector<int> activePids(LinuxParser::Pids());

    // get all existing pids
    vector<int> existingPids;
    existingPids.reserve(m_processes.size());
    for (const auto &process : m_processes)
        existingPids.push_back(process.Pid());

    // sort the active and existing pids for set operations
    std::sort(activePids.begin(), activePids.end());
    std::sort(existingPids.begin(), existingPids.end());

    // check which pids need to be removed (no longer active)
    vector<int> pidsToRemove;
    std::set_difference(existingPids.begin(), existingPids.end(), activePids.begin(), activePids.end(), std::back_inserter(pidsToRemove));

    // check which pids need to be added (newly active)
    vector<int> pidsToAdd;
    std::set_difference(activePids.begin(), activePids.end(), existingPids.begin(), existingPids.end(), std::back_inserter(pidsToAdd));
    m_processes.reserve(pidsToAdd.size() + m_processes.size());

    // remove old pids
    for (auto pid : pidsToRemove) {
        m_processes.erase(
            std::remove_if(m_processes.begin(), m_processes.end(), [pid](Process& x) { return x.Pid() == pid; }),
            m_processes.end()
            );
    }

    // update existing processes
    for (auto &process : m_processes) {
        process.update();
    }

    // insert new pids
    for (auto pid : pidsToAdd)
    {
        // if the new pids contain a launch command, we will include it, else, we will ignore
        // why? because it seems these pids don't have useful info like RAM and such
        // they are likely IRQs or subprocesses of other applications, idk?
        if (!LinuxParser::Command(pid).empty())
            m_processes.push_back(Process(pid));
    }
    // sort the array
    std::sort(m_processes.begin(), m_processes.end());
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