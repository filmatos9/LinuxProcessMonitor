#include "linux_parser.h"
#include "file_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  namespace fs = std::filesystem;
  vector<int> pids;
  for (const auto& entry : fs::directory_iterator(kProcDirectory)) {
    if (entry.is_directory()) {
      string filename(entry.path().filename());
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
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
  if (memFile.is_open()) {
    int memTotal{0};
    int memFree{0};
    // Get MemTotal
    if (FileParser::seekKey(memFile, MEM_TOTAL_KEY, delim)) {
      memFile >> memTotal;
    }
    if (FileParser::seekKey(memFile, MEM_AVAIL_KEY, delim)) {
      memFile >> memFree;
    }
    memFile.close();
    result = (memTotal-memFree)/static_cast<float>(memTotal);
  }
  return result;
}

long LinuxParser::UpTime() 
{ 
  std::ifstream upTimeFile(kProcDirectory + kUptimeFilename);
  long uptimeSecs = 0;
  if (upTimeFile.is_open()) {
    upTimeFile >> uptimeSecs;
    upTimeFile.close();
  }
  return uptimeSecs;
}

long LinuxParser::Jiffies() 
{ 
  return 0; 
}

long LinuxParser::ActiveJiffies(int pid)
{ 
  return 0; 
}

long LinuxParser::ActiveJiffies() 
{ 
  return 0; 
}

long LinuxParser::IdleJiffies() 
{ 
  return 0; 
}

vector<string> LinuxParser::CpuUtilization() 
{ 
  return {}; 
}

int LinuxParser::TotalProcesses() 
{ 
  const std::string TOTAL_PROC_KEY{"processes"};
  constexpr char delim = ' ';
  std::ifstream statFile(kProcDirectory + kStatFilename);
  int value{0};
  if (statFile.is_open())
  {
    if (FileParser::seekKey(statFile, TOTAL_PROC_KEY, delim)) {
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
    if (FileParser::seekKey(statFile, RUN_PROC_KEY, delim)) {
      statFile >> value;
    }
    statFile.close();
  }
  return value;
}

string LinuxParser::Command(int pid) 
{ 
  return string(); 
}

string LinuxParser::Ram(int pid) 
{ 
  return string(); 
}

string LinuxParser::Uid(int pid) 
{ 
  return string(); 
}

string LinuxParser::User(int pid) 
{ 
  return string(); 
}

long LinuxParser::UpTime(int pid) 
{ 
  return 0; 
}
