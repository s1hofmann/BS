// vim: set et ts=4 sw=4:

#include "meeting/bellringer.h"
#include "meeting/bell.h"

//Ohne die boolean wird ring_the_bells ziemlich oft aufgerufen
//Wurde wegrationalisiert
void Bellringer::check()
{
    bool jingleBells = false;

    Bell *bell;
    bell = bellList.dequeue();
    while(bell)
    {
        bell->tick();
        if(bell->run_down())
        {
            jingleBells = true;
        }
        bell = bellList.dequeue();
    }
    if(jingleBells)
    {
        ring_the_bells();
    }
}

//Die Glocke bell wird dem Glöckner überantwortet
//Wer schreibt den sowas?
void Bellringer::job(Bell *bell, int ticks)
{
    //Die Liste in check immer komplett durchlaufen ist blöd,
    //also gleich sortiert einhängen
    Bell *iter = bellList.first();
    Bell *oldIter;
    Bell *veryOldIter;

    if(!iter)
    {
        bellList.insert_first(bell);
        bell->wait(ticks);
    }
    else
    {
        int acc = 0;
        oldIter = iter;
        veryOldIter = oldIter;

        while(acc<ticks)
        {
            if(iter)
            {
                acc+=iter->wait();
                veryOldIter=oldIter;
                oldIter=iter;
                iter=iter->getnext();
            }
            else
            {
                bell->wait(ticks-acc);
                insert_after(oldIter, bell);
                return;
            }
        }
        acc=acc-oldIter->wait();
        int diff = ticks-acc;
        bell->wait(diff);
        insert_after(veryOldIter, bell);

        int oldAcc = acc;
        veryOldIter = veryOldIter->getnext();
        iter=veryOldIter;
        while(iter)
        {
            if(oldAcc+veryOldIter->wait()!=acc+iter->wait())
            {
                iter->wait(iter->wait()-diff);
            }
            iter=iter->getnext();
            
        }
    }
}

//Die Glocke soll nicht geläutet werden, also wird sie rausgeworfen.
void Bellringer::cancel(Bell *bell)
{
    remove(bell);
}

void Bellringer::ring_the_bells()
{
    Bell *bell;
    bell = bellList.dequeue();
    while(bell and bell->run_down())
    {
        bell->ring();
        bell = bellList.dequeue();
    }
}
