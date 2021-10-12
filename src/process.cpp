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

Process::Process(int pid) : m_pid{pid} {
    m_activeJiffies = LinuxParser::ActiveJiffies(pid);
    m_upTime = LinuxParser::UpTime(pid);
}

int Process::Pid()
{
    return m_pid;
}

float Process::CpuUtilization()
{
	float cpuUtil = 0.0f;
    // get idle and non-idle params
    long activeJiffies = LinuxParser::ActiveJiffies(m_pid);
    long upTime = LinuxParser::UpTime(m_pid);
    // calcuate difference
    long totald = upTime - m_upTime;
    long actived = activeJiffies - m_activeJiffies;
    // calculate percentage
    if (totald != 0)
        cpuUtil = (actived) / static_cast<float>(totald);
    // store new cpu utilization vals
    m_activeJiffies = activeJiffies;
    m_upTime = upTime;
	m_cpuUtilization = cpuUtil;
    return cpuUtil;
}

string Process::Command()
{
    return LinuxParser::Command(m_pid);
}

string Process::Ram()
{
    return LinuxParser::Ram(m_pid);
}

string Process::User()
{
    return LinuxParser::User(m_pid);
}

long int Process::UpTime()
{
    return m_upTime;
}

bool Process::operator<(Process const &a) const
{
    return m_activeJiffies < a.m_activeJiffies;
}