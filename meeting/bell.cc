    // vim: set et ts=4 sw=4:

#include "meeting/bell.h"
#include "thread/thread.h"
#include "thread/scheduler.h"
#include "meeting/bellringer.h"
#include "device/watch.h"

extern Scheduler scheduler;
extern Bellringer ringer;
extern Watch watch[];

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
    set(ms);
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
    int ticks = ms*1000;
    ticks/=watch[0].interval();
    ringer.cancel(this);
    ringer.job(this, ticks); 
}
    
void Bell::wait(int ticks)
{ 
    counter_ = ticks;
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
