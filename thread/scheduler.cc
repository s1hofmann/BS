// vim: set et ts=4 sw=4:
#define DEBUG

#include "thread/scheduler.h"
#include "thread/thread.h"
#include "thread/dispatch.h"

#include "object/debug.h"

void Scheduler::exit()
{
    //exit() soll den aktuellen Thread nicht wieder in die readyList einhängen, 
    //sondern einfach den ersten Thread in der readyList ausführen.
    Thread *then = readyList.dequeue();
    //Der dispatcher führt den Kontextwechsel durch
    dispatch(then);
}

void Scheduler::kill(Thread *t)
{
    //Thread aus readyList löschen
    Thread *victim = readyList.remove(t);
    if(!victim)
    {
        //Thread wurde nicht in readyList gefunden -> kill_flag setzen
        t->set_kill_flag();
    }
}

void Scheduler::ready(Thread *t)
{
    t->reset_kill_flag();
    readyList.enqueue(t);
}

void Scheduler::resume()
{
    Thread *now = active();
    readyList.enqueue(now);
    //Eventuell gekillte Threads überspringen
    Thread *then;
    then = readyList.dequeue();
    while(then->dying())
    {
        then = readyList.dequeue();
    }
    dispatch(then);
}

void Scheduler::schedule()
{
    //Setzt Scheduling in Gang.
    //Dabei handelt es sich um den ERSTEN Aufruf überhaupt, go() muss verwendet werden
    //Alle weiteren Kontextwechsel werden über dispatch() gemacht
    Thread *start = readyList.dequeue();
    go(start);
}
