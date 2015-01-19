#include "meeting/semaphore.h"
#include "thread/scheduler.h"

extern Scheduler scheduler;

void Semaphore::p()
{
    if(this->counter_>0)
    {
        --this->counter_;
    }
    else
    {
        scheduler.block(scheduler.active(), this->waiting_);
    }
}

void Semaphore::v()
{
}
