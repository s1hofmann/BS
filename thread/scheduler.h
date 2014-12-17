// vim: set et ts=4 sw=4:

#ifndef __schedule_include__
#define __schedule_include__

/*! \file
 *
 *  \brief Enthält die Klasse Scheduler
 */

#include "thread/dispatch.h"
#include "object/queue.h"
#include "machine/spinlock.h"

/*! \brief Implementierung des Schedulers.
 *
 *  Der Scheduler verwaltet die Ready-Liste (ein privates Queue Objekt der
 *  Klasse), also die Liste der lauffähigen Threads. Die
 *  Liste wird von vorne nach hinten abgearbeitet. Dabei werden Threads, die
 *  neu im System sind oder den Prozessor abgeben, stets an das Ende der Liste
 *  angefügt.
 */
class Scheduler : public Dispatcher
{
public:
    Scheduler() {}
    void exit();
    void kill(Thread *t);
    void ready(Thread *t);
    void resume();
    void schedule();

private:
    Scheduler (const Scheduler &copy); // Verhindere Kopieren
    Queue<Thread> readyList;
    Spinlock threadLock;
};

#endif
