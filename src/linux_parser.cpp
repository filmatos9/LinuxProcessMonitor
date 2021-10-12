#include "linux_parser.h"
#include "file_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem()
{
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value)
            {
                if (key == "PRETTY_NAME")
                {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

string LinuxParser::Kernel()
{
    string os, kernel, version;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

vector<int> LinuxParser::Pids()
{
    namespace fs = std::filesystem;
    vector<int> pids;
    for (const auto &entry : fs::directory_iterator(kProcDirectory))
    {
        if (entry.is_directory())
        {
            string filename(entry.path().filename());
            if (std::all_of(filename.begin(), filename.end(), isdigit))
            {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    return pids;
}

float LinuxParser::MemoryUtilization()
{
    const std::string MEM_TOTAL_KEY{"MemTotal"};
    const std::string MEM_AVAIL_KEY{"MemAvailable"};
    constexpr char delim = ':';
    std::ifstream memFile(kProcDirectory + kMeminfoFilename);
    float result{0.0f};
    // check if the file is open
    if (memFile.is_open())
    {
        int memTotal{0};
        int memFree{0};
        // Get MemTotal
        if (FileParser::seekKey(memFile, MEM_TOTAL_KEY, delim))
        {
            memFile >> memTotal;
        }
        if (FileParser::seekKey(memFile, MEM_AVAIL_KEY, delim))
        {
            memFile >> memFree;
        }
        memFile.close();
        result = (memTotal - memFree) / static_cast<float>(memTotal);
    }
    return result;
}

int LinuxParser::TotalProcesses()
{
    const std::string TOTAL_PROC_KEY{"processes"};
    constexpr char delim = ' ';
    std::ifstream statFile(kProcDirectory + kStatFilename);
    int value{0};
    if (statFile.is_open())
    {
        if (FileParser::seekKey(statFile, TOTAL_PROC_KEY, delim))
        {
            statFile >> value;
        }
        statFile.close();
    }
    return value;
}

int LinuxParser::RunningProcesses()
{
    const std::string RUN_PROC_KEY{"procs_running"};
    constexpr char delim = ' ';
    std::ifstream statFile(kProcDirectory + kStatFilename);
    int value{0};
    if (statFile.is_open())
    {
        if (FileParser::seekKey(statFile, RUN_PROC_KEY, delim))
        {
            statFile >> value;
        }
        statFile.close();
    }
    return value;
}

string LinuxParser::Command(int pid)
{
    string command;
    std::ifstream cmdFile(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (cmdFile.is_open())
    {
        // read the full file
        std::ostringstream buffer;
        buffer << cmdFile.rdbuf();
        command = buffer.str();
        cmdFile.close();
    }
    return command;
}

string LinuxParser::Ram(int pid)
{
    string memUsage;
    std::ifstream statusFile(kProcDirectory + to_string(pid) + kStatusFilename);
    if (statusFile.is_open())
    {
        if (FileParser::seekKey(statusFile, "VmSize", ':'))
        {
            statusFile >> memUsage;
        }
        statusFile.close();
    }
    return memUsage;
}

string LinuxParser::Uid(int pid)
{
    std::string uid;
    std::ifstream statusFile(kProcDirectory + to_string(pid) + kStatusFilename);
    if (statusFile.is_open())
    {
        if (FileParser::seekKey(statusFile, "Uid", ':'))
        {
            statusFile >> uid;
        }
        statusFile.close();
    }
    return uid;
}

string LinuxParser::User(int pid)
{
    std::string user;
    std::string uid = Uid(pid);
    if (!uid.empty()) {
        std::ifstream pwdFile(kPasswordPath);
        if (pwdFile.is_open())
        {
            std::string line;
            while (getline(pwdFile, line))
            {
                if (line.find(uid) != line.npos)
                {
                    std::istringstream linestream(line);
                    getline(linestream, user, ':');
                    break;
                }
            }
            pwdFile.close();
        }
    }
    return user;
}

long LinuxParser::UpTime()
{
    std::ifstream upTimeFile(kProcDirectory + kUptimeFilename);
    long uptimeSecs = 0;
    if (upTimeFile.is_open())
    {
        upTimeFile >> uptimeSecs;
        upTimeFile.close();
    }
    return uptimeSecs;
}

long LinuxParser::UpTime(int pid)
{
    constexpr int upTimeIndex = 22;
    std::ifstream upTimeFile(kProcDirectory + to_string(pid) + kStatFilename);
    long uptimeSecs = 0;
    if (upTimeFile.is_open())
    {
        std::string upTimeStr;
        int val = 0;
        while(val++ < upTimeIndex && upTimeFile >> upTimeStr);
        uptimeSecs = stol(upTimeStr);
        uptimeSecs /= sysconf(_SC_CLK_TCK);
        upTimeFile.close();
    }
    return uptimeSecs;
}

// CPU UTILIZATION

long LinuxParser::ActiveJiffies(int pid)
{
    return 0;
}

LinuxParser::CPU_Utilization::CPU_Utilization()
    : user{0l}, nice{0l}, system{0l}, idle{0l}, iowait{0l}, irq{0l}, softirq{0l}, steal{0l}, guest{0l}, guest_nice{0l}
{
}

LinuxParser::CPU_Utilization::CPU_Utilization(const vector<string> &strVec) : LinuxParser::CPU_Utilization()
{
    if (strVec.size() > 9)
    {
        user = stol(strVec[LinuxParser::kUser_]);
        nice = stol(strVec[LinuxParser::kNice_]);
        system = stol(strVec[LinuxParser::kSystem_]);
        idle = stol(strVec[LinuxParser::kIdle_]);
        iowait = stol(strVec[LinuxParser::kIOwait_]);
        irq = stol(strVec[LinuxParser::kIRQ_]);
        softirq = stol(strVec[LinuxParser::kSoftIRQ_]);
        steal = stol(strVec[LinuxParser::kSteal_]);
        guest = stol(strVec[LinuxParser::kGuest_]);
        guest_nice = stol(strVec[LinuxParser::kGuestNice_]);
    }
}

long LinuxParser::CPU_Utilization::getIdle()
{
    return idle + iowait;
}

long LinuxParser::CPU_Utilization::getNonIdle()
{
    return user + nice + system + irq + softirq + steal;
}

LinuxParser::CPU_Utilization LinuxParser::CpuUtilization(int cpuNum)
{
    const std::string CPU_KEY_ROOT{"cpu"};
    constexpr char delim = ' ';
    std::string CPU_KEY{CPU_KEY_ROOT};
    if (cpuNum != -1)
        CPU_KEY += to_string(cpuNum);

    std::ifstream statFile(kProcDirectory + kStatFilename);
    vector<string> cpuUtilVals;
    cpuUtilVals.reserve(10);
    if (statFile.is_open())
    {
        if (FileParser::seekKey(statFile, CPU_KEY, delim))
        {
            string val;
            while ((statFile.peek() != '\n') && statFile >> val)
                cpuUtilVals.push_back(val);
        }
        statFile.close();
    }
    return LinuxParser::CPU_Utilization(cpuUtilVals);
}
