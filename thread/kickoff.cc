// vim: set et ts=4 sw=4:

#include "object/debug.h"

#include "thread/thread.h"

void kickoff(Thread *thread)
{
    //Kickoff soll nie terminieren
    while(true)
    {
        thread->action();
    }
}
