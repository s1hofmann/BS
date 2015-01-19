// vim: set et ts=4 sw=4:

#include "meeting/waitingroom.h"
#include "thread/scheduler.h"
#include "machine/apicsystem.h"
#include "machine/plugbox.h"

extern Scheduler scheduler;
extern APICSystem system;
extern Plugbox plugbox;

void Waitingroom::add(Thread *t)
{
    this->enqueue(t);
}

void Waitingroom::remove(Thread *t)
{
    scheduler.ready(this->remove(t));
}

Waitingroom::~Waitingroom()
{
    Thread *t = this->dequeue();
    int logicalDestination=1;

    while(this->first())
    {
        scheduler.ready(t);
        for(int i=0; i<CPU_MAX; ++i)
        {
            system.sendCustomIPI(logicalDestination<<i, Plugbox::wakeup);
        }
        Thread *t = this->dequeue();
    }
}
