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

template<typename K, typename V>
std::vector<std::pair<K, V>> mapToVector(const std::unordered_map<K, V> &map)
{
    std::vector<V> v;
    v.resize(map.size());
 
    std::copy(map.begin(), map.end(), v.begin());
 
    return v;
}

vector<Process> &System::Processes()
{
    vector<int> pids(LinuxParser::Pids());

    // insert new pids
    for (auto pid : pids) 
    {
        if (m_processes.find(pid) == m_processes.end()){
            m_processes.insert( { pid, Process(pid) } );
        }
    }
    // create result vector
    vector<Process> result;
    result.reserve(m_processes.size());
    result = mapToVector(m_processes);
    std::sort(result.begin(), result.end());
    return result;
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