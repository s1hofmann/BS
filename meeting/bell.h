// vim: set et ts=4 sw=4:

#include "meeting/waitingroom.h"

#ifndef __Bell_include__
#define __Bell_include__

#define DEBUG

#include "object/debug.h"

/*! \file
 *  \brief Enthält die Klasse Bell.
 */

/*! \brief Synchronisationsobjekt zum Schlafenlegen für eine bestimmte
 *  Zeitspanne
 *
 *  Ein "Wecker" ist ein Synchronisationsobjekt, mit dem ein oder mehrere
 *  Threads sich für eine bestimmte Zeit schlafen legen können.
 */
class Bell : public Waitingroom, public QueueEntry<Bell>
{
public:
    void ring() { DBG << "GET UP!" << endl; }
    void set(int ms) { }
    void sleep() {}
    void sleep(int ms) { counter_ = ms; sleep(); }
    void wait(int ms) { counter_ = ms; }
    
    int wait() { return counter_; }
    void tick() { --counter_; }
    bool run_down() { return counter_==0 ? true : false; }
    
private:
    Bell(const Bell &copy); // Verhindere Kopieren
    int counter_;
};

#endif
