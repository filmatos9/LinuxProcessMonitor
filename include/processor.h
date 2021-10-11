#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor 
{
public:
    Processor();
    float Utilization();

private:
    long m_idleJiffies;
    long m_activeJiffies;
};

#endif