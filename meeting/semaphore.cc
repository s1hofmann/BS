#define DEBUG

#include "meeting/semaphore.h"
#include "thread/scheduler.h"

#include "object/debug.h"

extern Scheduler scheduler;

void Semaphore::p()
{
    if(this->counter_>0)
    {
        --this->counter_;
    }
    else
    {
        Thread *t = scheduler.active();
        //DBG << "blocked t: " << t->getID() << endl;
        scheduler.block(t, this);
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
