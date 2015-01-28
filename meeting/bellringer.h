// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Bellringer.
 */

#ifndef __Bellringer_include__
#define __Bellringer_include__

#include "object/queue.h"
#include "meeting/bell.h"

/*! \brief Verwaltung und Anstoßen von zeitgesteuerten Aktivitäten.
 *
 *  Der "Glöckner" (Bellringer) wird regelmäßig aktiviert und prüft, ob
 *  irgendwelche "Glocken" (Bell-Objekte) läuten müssen. Die Glocken befinden
 *  sich in einer Liste, die der Glöckner verwaltet. Eine geschickte
 *  Implementierung vermeidet, bei jeder Aktivierung die gesamte Liste
 *  zu durchlaufen.
 */
class Bellringer : public Queue<Bell>
{
private:
    Bellringer(const Bellringer &copy); // Verhindere Kopieren

    Queue<Bell> bellList;

public:
    /*! \brief Konstruktor.
     */
    Bellringer() {}
    void check();
    void job(Bell *b, int ticks);
    void cancel(Bell *b);
    void ring_the_bells();
};

#endif
