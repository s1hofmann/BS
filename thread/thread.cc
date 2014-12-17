// vim: set et ts=4 sw=4:

#include "thread/thread.h"

Thread::Thread(void *tos) : kill_flag(false), tos_(tos)
{
    //Kontext aufsetzen, object ist aktuelles Objekt
    toc_settle(&registers, tos, &kickoff, this);
}

void Thread::go()
{
    toc_go(&registers);
}

void Thread::resume(Thread *next)
{
    toc_switch(&registers, &(next->registers));
}
