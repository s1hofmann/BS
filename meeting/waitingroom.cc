// vim: set et ts=4 sw=4:

#include "meeting/waitingroom.h"
#include "thread/scheduler.h"
#include "machine/apicsystem.h"
#include "machine/plugbox.h"

extern Scheduler scheduler;
extern APICSystem system;
extern Plugbox plugbox;

void Waitingroom::remove(Thread *t)
{
    Queue::remove(t);
}

Waitingroom::~Waitingroom()
{
    Thread *t;
    int logicalDestination=1;

    while((t = this->dequeue()))
    {
        scheduler.ready(t);
        for(int i=0; i<CPU_MAX; ++i)
        {
            system.sendCustomIPI(logicalDestination<<i, Plugbox::wakeup);
        }
    }
}
