#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process
{
  public:
    Process(int pid);
    int Pid() const;
    std::string User() const;
    std::string Command() const;
    float CpuUtilization() const;
    std::string Ram() const;
    long int UpTime() const;
    bool operator<(Process const &a) const;
    void update();

  private:
    int m_pid;
    float m_activeTimeSecs;
    long m_upTimeSecs;
    float m_cpuUtilization;
    std::string m_command;
    std::string m_user;
    std::string m_ram;
    
    void updateCpuUtilization();
    void updateRam();
};

#endif