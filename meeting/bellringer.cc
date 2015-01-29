// vim: set et ts=4 sw=4:

#include "meeting/bellringer.h"
#include "meeting/bell.h"

void Bellringer::check()
{
}

void Bellringer::job(Bell *bell, int ticks)
{
}

void Bellringer::cancel(Bell *bell)
{
    remove(bell);
}

void Bellringer::ring_the_bells()
{
    Bell *bell;
    bell = bellList.dequeue();
    while(bell->run_down())
    {
        bell->ring();
        bell = bellList.dequeue();
    }
}
