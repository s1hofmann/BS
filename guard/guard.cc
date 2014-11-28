
// vim: set et ts=4 sw=4:

#include "guard/guard.h"

void Guard::enter()
{
    while(!avail())
    {
    }
    enter();
}

void Guard::leave()
{
    retne();
}

void Guard::relay(Gate *item)
{
}
