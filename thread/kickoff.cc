// vim: set et ts=4 sw=4:

#include "object/debug.h"

#include "thread/thread.h"
#include "guard/guard.h"

extern Guard guard;

void kickoff(Thread *thread)
{
    guard.leave();
    //Kickoff soll nie terminieren
    while(true)
    {
        thread->action();
    }
}
