// vim: set et ts=4 sw=4:

#define DEBUG
#include "meeting/bellringer.h"
#include "meeting/bell.h"

#include "object/debug.h"
//Ohne die boolean wird ring_the_bells ziemlich oft aufgerufen
//Wurde wegrationalisiert
void Bellringer::check()
{
    DBG << "ringer check" << endl;

    bool jingleBells = false;

    Bell *bell;

    bell = bellList.first();

    bell->tick();

    if(bell->run_down())
    {
        jingleBells = true;
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

    // Wenn bellList leer, dann an erster Stelle einfuegen
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

        // Solange Akkumulator kleiner als Ticks, durch Liste hangeln
        // Andernfalls ist Position zum Einfuegen erreicht
        while(acc<ticks)
        {

            // Ende der Liste erreicht? -> Einfuegen!
            if(!iter)
            {
                bell->wait(ticks-acc);
                insert_after(oldIter, bell);
                return;
            }

            acc+=iter->wait();
            veryOldIter=oldIter;
            oldIter=iter;
            iter=iter->getnext();
        }

        // Vorigen Akkumulator wieder herstellen
        acc=acc-oldIter->wait();
        int diff = ticks-acc;
        bell->wait(diff);
        insert_after(veryOldIter, bell);

        veryOldIter = veryOldIter->getnext();
        iter=veryOldIter;

        while(iter)
        {
            if(0 != iter->wait())
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
    DBG << "ring ring" << endl;
    Bell *bell = bellList.dequeue();
    while(bell and bell->run_down())
    {
        bell->ring();
        bell = bellList.dequeue();
    }
}
