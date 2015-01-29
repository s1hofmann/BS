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

    Bell *bell;

    bell = first();

    bell->tick();

    if(bell->run_down())
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
    Bell *iter = first();
    Bell *oldIter;
    //Bell *veryOldIter;

    // Wenn bellList leer, dann an erster Stelle einfuegen
    if(!iter)
    {
        bell->wait(ticks);
        insert_first(bell);
        return;
    }
    else
    {
        //int acc = 0;
        oldIter = iter;
        //veryOldIter = oldIter;

        //Von ticks sollen alle vorherigen counter abgezogen werden
        //-> iterieren über die liste solange es ein element iter gibt und sein bell->wait() größer ist als iter->wait()
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

        // Solange Akkumulator kleiner als Ticks, durch Liste hangeln
        // Andernfalls ist Position zum Einfuegen erreicht
        //while(acc<ticks)
        //{

            //// Ende der Liste erreicht? -> Einfuegen!
            //if(!iter)
            //{
                //bell->wait(ticks-acc);
                //insert_after(oldIter, bell);
                //return;
            //}

            //acc+=iter->wait();
            //veryOldIter=oldIter;
            //oldIter=iter;
            //iter=iter->getnext();
        //}

        //// Vorigen Akkumulator wieder herstellen
        //acc=acc-oldIter->wait();
        //int diff = ticks-acc;
        //bell->wait(diff);
        //insert_after(veryOldIter, bell);

        //veryOldIter = veryOldIter->getnext();
        //iter=veryOldIter;

        //while(iter)
        //{
            //if(0 != iter->wait())
            //{
                //iter->wait(iter->wait()-diff);
            //}
            //iter=iter->getnext();
        //}
    }
}

//Die Glocke soll nicht geläutet werden, also wird sie rausgeworfen.
//
//Hier wird doch was aus der Liste geworfen, also ist die Sortierung wieder am Arsch?
//Die Elemente nach der Bell müssen aktualisiert werden?
void Bellringer::cancel(Bell *bell)
{
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
