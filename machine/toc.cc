// vim: set et ts=4 sw=4:

#include "machine/toc.h"
#include "thread/thread.h"

// TOC_SETTLE: bereitet den Kontext der Koroutine fuer den ersten Aufruf vor.
void toc_settle (struct toc *regs, void *tos, void (*kickoff)(Thread*), Thread *object)
{
    void **sp = (void**)tos;
    *(--sp) = object;
    *(--sp) = (void*)42;
    *(--sp) = (void*)kickoff;
    regs->esp = sp;
}
