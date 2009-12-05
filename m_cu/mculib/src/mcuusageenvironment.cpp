
#include "mcuusageenvironment.h"
#include "log.h"

CMCUUsageEnvironment::CMCUUsageEnvironment( TaskScheduler& taskScheduler ) :
BasicUsageEnvironment( taskScheduler )
{

}

CMCUUsageEnvironment::~CMCUUsageEnvironment()
{

}


CMCUUsageEnvironment *CMCUUsageEnvironment::createNew(TaskScheduler& taskScheduler)
{
    return new CMCUUsageEnvironment( taskScheduler );
}


UsageEnvironment& CMCUUsageEnvironment::operator<<(char const* str) {
    mcu::log << str;
    return *this;
}

UsageEnvironment& CMCUUsageEnvironment::operator<<(int i) {
//    fprintf(stderr, "%d", i);
    mcu::log << i;
    return *this;
}

UsageEnvironment& CMCUUsageEnvironment::operator<<(unsigned u) {
//    fprintf(stderr, "%u", u);
    mcu::log << u;
    return *this;
}

UsageEnvironment& CMCUUsageEnvironment::operator<<(double d) {
//    fprintf(stderr, "%f", d);
    mcu::log << d;
    return *this;
}

UsageEnvironment& CMCUUsageEnvironment::operator<<(void* p) {
//    fprintf(stderr, "%p", p);
    mcu::log << p;
    return *this;
}

