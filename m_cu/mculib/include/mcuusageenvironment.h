#pragma once
#include "basicusageenvironment.hh"

class CMCUUsageEnvironment :
    public BasicUsageEnvironment
{
public:
    static CMCUUsageEnvironment* createNew(TaskScheduler& taskScheduler);
public:
    CMCUUsageEnvironment( TaskScheduler& taskScheduler );
    virtual ~CMCUUsageEnvironment(void);

    virtual UsageEnvironment& operator<<(char const* str);
    virtual UsageEnvironment& operator<<(int i);
    virtual UsageEnvironment& operator<<(unsigned u);
    virtual UsageEnvironment& operator<<(double d);
    virtual UsageEnvironment& operator<<(void* p);
};
