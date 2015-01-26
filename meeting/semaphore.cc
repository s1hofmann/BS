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
        scheduler.block(scheduler.active(), this);
    }
}

void Semaphore::v()
{
    Thread *t;
    if((t = this->dequeue()))
    {
        scheduler.wakeup(t);
    }
    else
    {
        ++this->counter_;
    }
}
