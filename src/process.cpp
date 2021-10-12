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
}

int Process::Pid()
{
    return m_pid;
}

float Process::CpuUtilization()
{
	return 0;
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
    return LinuxParser::UpTime(m_pid);
}

bool Process::operator<(Process const &a) const
{
    return m_pid < a.m_pid;
}