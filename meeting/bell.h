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
    Bell() {};
    ~Bell() {};
    void ring();
    void sleep();
    void set(int ms);
    void sleep(int ms);

    void wait(int ms);
    int wait();

    void tick();
    bool run_down();

private:
    Bell(const Bell &copy); // Verhindere Kopieren
    int counter_;
};

#endif
