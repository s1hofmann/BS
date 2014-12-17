// vim: set et ts=4 sw=4:

#include "thread/dispatch.h"

void Dispatcher::go(Thread *first)
{
    setActive(first);
    first->go();
}

void Dispatcher::dispatch(Thread *then)
{
    //Resume kann nur von einem anderen Thread aufgerufen werden
    // -> aktuellen Thread zwischenspeichern und für resume() verwenden
    Thread *now = active();
    setActive(then);
    now->resume(then);
}

void Dispatcher::setActive(Thread *c)
{
    life[system.getCPUID()] = c;
}
