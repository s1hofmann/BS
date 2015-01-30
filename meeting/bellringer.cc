// vim: set et ts=4 sw=4:

#define DEBUG
#include "meeting/bellringer.h"
#include "meeting/bell.h"

#include "object/debug.h"
//Ohne die boolean wird ring_the_bells ziemlich oft aufgerufen
//Wurde wegrationalisiert
void Bellringer::check()
{
    Bell *bell;
    bell = first();

    while(bell)
    {
        if(bell->run_down())
        {
            dequeue();
            bell->ring();
            bell = first();
        }
        else
        {
            bell->tick();
            return;
        }
    }
}

//Die Glocke bell wird dem Glöckner überantwortet
//Wer schreibt den sowas?
void Bellringer::job(Bell *bell, int ticks)
{
    //Die Liste in check immer komplett durchlaufen ist blöd,
    //also gleich sortiert einhängen
    Bell *iter = first();
    Bell *oldIter = 0;

    if(!iter)
    {
        bell->wait(ticks);
        insert_first(bell);
        return;
    }
    else
    {
        //Von ticks sollen alle vorherigen counter abgezogen werden
        //-> iterieren über die liste solange es ein element iter gibt und bell->wait() größer ist als iter->wait()
        //und dann bell->wait() auf bell->wait() - iter->wait() setzen
        
        while(iter and bell->wait() > iter->wait())
        {
            bell->wait(bell->wait()-iter->wait());
            oldIter = iter;
            iter = iter->getnext();
        }

        if(oldIter)
        {
            insert_after(oldIter, bell);
        }
        else
        {
            insert_first(bell);
        }

        //Das Element NACH der eingefügten Bell muss auch noch aktualisiert werden
        iter->wait(iter->wait()-bell->wait());
    }
}

//Die Glocke soll nicht geläutet werden, also wird sie rausgeworfen.
//
//Hier wird doch was aus der Liste geworfen, also ist die Sortierung wieder am Arsch?
//Die Elemente nach der Bell müssen aktualisiert werden?
void Bellringer::cancel(Bell *bell)
{
    Bell *iter = first();
    while(iter and iter!=bell)
    {
        iter = iter->getnext();
    }
    iter=iter->getnext();
    if(iter)
    {
        iter->wait(iter->wait()+bell->wait());
    }
    remove(bell);
}

void Bellringer::ring_the_bells()
{
    DBG << "ring ring" << endl;
    Bell *bell = dequeue();
    while(bell and bell->run_down())
    {
        bell->ring();
        bell = dequeue();
    }
}
