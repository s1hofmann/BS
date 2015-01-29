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
    //Counter der Bell einstellen, da sie ja 'überantwortet' wurde
    bell->wait(ticks);

    //Die Liste in check immer komplett durchlaufen ist blöd,
    //also gleich sortiert einhängen
    Bell *iter = bellList.first();
    if(!iter or iter->wait()>ticks)
    {
        bellList.insert_first(bell);
    }
    else
    {
        while(iter->wait()<ticks)
        {
            if(!iter->getnext())
            {
                insert_after(iter, bell);
                return;
            }
            else
            {
                iter = iter->getnext();
            }
        }
        insert_after(iter, bell);
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
