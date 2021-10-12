#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

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