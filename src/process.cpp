#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "file_parser.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : m_pid{pid}
{
    m_command = LinuxParser::Command(pid);
    m_user = LinuxParser::User(m_pid);
    m_activeTimeSecs = static_cast<float>(LinuxParser::ActiveJiffies(m_pid)) / sysconf(_SC_CLK_TCK);
    m_upTimeSecs = LinuxParser::UpTime(pid);
    // use average cpu utilization to start
    if (m_upTimeSecs > 0)
        m_cpuUtilization = m_activeTimeSecs / m_upTimeSecs;
    else
        m_cpuUtilization = 0.0f;
}

int Process::Pid() const
{
    return m_pid;
}

float Process::CpuUtilization() const
{
    return m_cpuUtilization;
}

string Process::Command() const
{
    return m_command;
}

string Process::Ram() const
{
    return m_ram;
}

string Process::User() const
{
    return m_user;
}

long Process::UpTime() const
{
    return m_upTimeSecs;
}

bool Process::operator<(Process const &a) const
{
    return m_cpuUtilization > a.m_cpuUtilization;
}

void Process::update(){
    updateCpuUtilization();
    updateRam();
}

void Process::updateCpuUtilization() {
    // get idle and non-idle params
    float activeTimeSecs = static_cast<float>(LinuxParser::ActiveJiffies(m_pid)) / sysconf(_SC_CLK_TCK);
    long upTimeSecs = LinuxParser::UpTime(m_pid);
    // calcuate difference
    long totald = upTimeSecs - m_upTimeSecs;
    float actived = activeTimeSecs - m_activeTimeSecs;
    // calculate percentage
    if (totald > 0)
    {
        m_cpuUtilization = actived / totald;
        m_activeTimeSecs = activeTimeSecs;
        m_upTimeSecs = upTimeSecs;
    }
}

void Process::updateRam()
{
    m_ram = LinuxParser::Ram(m_pid);
}