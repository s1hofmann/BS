// vim: set et ts=4 sw=4:

#include "meeting/bell.h"
#include "thread/thread.h"
#include "thread/scheduler.h"
#include "meeting/bellringer.h"

extern Scheduler scheduler;
extern Bellringer ringer;

void Bell::sleep()
{
    //DBG << "sleep" << endl;
    if(counter_>0)
    {
        enqueue(scheduler.active());
        scheduler.block(this);
    }
}

void Bell::sleep(int ms)
{
    counter_ = ms;
    sleep();
}

void Bell::ring()
{
    //DBG << "GET UP!" << endl;
    Thread *iter = dequeue();
    while(iter)
    {
        scheduler.wakeup(iter);
        iter = dequeue();
    }
}
    
void Bell::set(int ms) 
{ 
    ringer.cancel(this);
    ringer.job(this, ms); 
}
    
void Bell::wait(int ms)
{ 
    counter_ = ms;
}

int Bell::wait()
{
    return counter_;
}

void Bell::tick()
{
    --counter_;
}

bool Bell::run_down()
{
    return counter_ <= 0 ? true : false;
}
