// vim: set et ts=4 sw=4:
#define DEBUG

#include "thread/scheduler.h"
#include "thread/thread.h"
#include "thread/idlethread.h"
#include "thread/dispatch.h"
#include "machine/apicsystem.h"
#include "machine/plugbox.h"

#include "meeting/waitingroom.h"

#include "guard/guard.h"

#include "object/debug.h"

extern Guard guard;
extern APICSystem system;
extern IdleThread idleThreads[];

void Scheduler::kill(Thread *t)
{
    //Thread aus readyList löschen
    Thread *victim = readyList.remove(t);
    if(!victim)
    {
        if(t->waiting_in())
        {
            (t->waiting_in())->remove(t);
        }
        //Thread wurde nicht in readyList gefunden -> kill_flag setzen
        t->set_kill_flag();

        //Thread auf anderen CPUs suchen
        int logicalDestination = 1;
        for(int i=0; i<CPU_MAX; ++i)
        {
            if(life[i]==victim)
            {
                //Logicaldestination durch lapic erhalten
                system.sendCustomIPI(logicalDestination<<i, Plugbox::assassin);
            }
        }
    }
}

void Scheduler::ready(Thread *t)
{
    t->reset_kill_flag();
    readyList.enqueue(t);
    incrementThreadCount();

    //Thread auf anderen CPUs suchen
    int logicalDestination = 1;
    for(int i=0; i<CPU_MAX; ++i)
    {
        //Wenn ein neuer Thread aktiv wird sollen alle CPUs geweckt werden
        //
        if(i!=system.getCPUID())
        {
            //Logicaldestination durch lapic erhalten
            system.sendCustomIPI(logicalDestination<<i, Plugbox::wakeup);
        }
    }
}

void Scheduler::exit()
{
    //exit() soll den aktuellen Thread nicht wieder in die readyList einhängen, 
    //sondern einfach den ersten Thread in der readyList ausführen.
    //Der dispatcher führt den Kontextwechsel durch
    Thread *then = readyList.dequeue();
    while(then and then->dying())
    {
        then = readyList.dequeue();
    }

    if(then)
    {
        dispatch(then);
    }
    else
    {
        //DBG << "readyList empty" << endl;
        dispatch(idleThreads[system.getCPUID()]);
    }
}

void Scheduler::resume()
{
    Thread *now = active();
    //Wenn der Thread sterben soll, sollte er auch nicht wieder eingefügt werden
    if(now != idleThreads[system.getCPUID()] and !now->dying())
    {
        readyList.enqueue(now);
    }
    //Alles, was hier gemacht wird, wird doch quasi eh schon fast in exit() gemacht
    //Dann könnte man das doch kombinieren
    
        //Eventuell gekillte Threads überspringen
        //Thread *then;
        //then = readyList.dequeue();
        //while(then and then->dying())
        //{
            //then = readyList.dequeue();
        //}
        //dispatch(then);
    //}
    //else
    //{
        //DBG << "readyList empty" << endl;
        //dispatch(idleThreads[system.getCPUID()]);
    //}
    exit();
}

void Scheduler::schedule()
{
    //Setzt Scheduling in Gang.
    //Dabei handelt es sich um den ERSTEN Aufruf überhaupt, go() muss verwendet werden
    //Alle weiteren Kontextwechsel werden über dispatch() gemacht
    Thread *start = readyList.dequeue();
    if(start)
    {
        go(start);
    }
    else
    {
        DBG << "readyList empty" << endl;
        go(idleThreads[system.getCPUID()]);
    }
}

//Sollte der Thread nicht beim Waitingroom enqueued werden?
void Scheduler::block(Waitingroom *w)
{
    Thread *t = active();
    //w->enqueue(t);
    t->waiting_in(w);
    exit();
}

void Scheduler::wakeup(Thread *t)
{
    //if(t->waiting_in())
    //{
        //t->waiting_in()->remove(t);
    //}
    t->waiting_in(0);
    this->ready(t);
}
    
bool Scheduler::isEmpty()
{
    //Ich hoffe jetzt einfach mal es wird 0 zuückgegeben, wenn die Liste leer ist
    return readyList.first();
}
void Scheduler::setIdleThread(int cpuId, Thread* idleThread)
{
    this->idleThreads[cpuId] = idleThread;
}
