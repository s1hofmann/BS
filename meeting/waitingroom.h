// vim: set et ts=4 sw=4:

#ifndef __waitingroom_include__
#define __waitingroom_include__

#include "object/queue.h"
#include "thread/thread.h"

/*! \file
 *  \brief Enthält die Klasse Waitingroom.
 */

/*! \brief Liste von Threads, die auf ein Ereignis warten.
 *
 *  Die Klasse Waitingroom implementiert eine Liste von Threads, die alle auf
 *  ein bestimmtes Ereignis warten.
 *  \note
 *  Die Methode remove(Thread*) muss virtuell sein, damit der Scheduler einen
 *  Thread aus dem Wartezimmer entfernen kann, ohne wissen zu müssen, welcher
 *  Art dieses Wartezimmer ist. Sofern es erforderlich ist, kann eine von
 *  Waitingroom abgeleitete Klasse die Methode auch noch neu definieren.
 *
 *  Der Destruktor sollte wie bei allen Klassen, die virtuelle Methoden
 *  definieren, ebenfalls virtuell sein.
 */
class Waitingroom : public Queue<Thread>
{
private:
    Waitingroom(const Waitingroom &copy); // Verhindere Kopieren

public:
    Waitingroom() {}
    virtual void add(Thread *t);
    virtual void remove(Thread *t);
    virtual ~Waitingroom();
};

#endif
