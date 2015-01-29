// vim: set et ts=4 sw=4:

#include "meeting/bell.h"
#include "thread/thread.h"
#include "thread/scheduler.h"
#include "meeting/bellringer.h"

extern Scheduler scheduler;
extern Bellringer ringer;

void Bell::sleep()
{
    DBG << "sleep" << endl;
    ringer.job(this, startTime_);
    scheduler.block(this);
}

void Bell::ring()
{
    DBG << "GET UP!" << endl;
    Thread *iter = dequeue();
    while(iter)
    {
        scheduler.ready(iter);
        iter = dequeue();
    }
}
