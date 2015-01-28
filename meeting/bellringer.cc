// vim: set et ts=4 sw=4:

#include "meeting/bellringer.h"
#include "meeting/bell.h"

void Bellringer::check()
{
}

void Bellringer::job(Bell *b)
{
}

void Bellringer::cancel(Bell *b)
{
    b->sleep();
}

void Bellringer::ring_the_bells()
{
    Bell *b;
    b = bellList.dequeue();
    while(b->run_down())
    {
        b->ring();
        b = bellList.dequeue();
    }
}
