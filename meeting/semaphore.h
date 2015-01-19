// vim: set et ts=4 sw=4:

#ifndef __Semaphore_include__
#define __Semaphore_include__

#include "meeting/waitingroom.h"
#include "thread/thread.h"

/*! \file
 *  \brief Enthält die Klasse Semaphore.
 */

/*! \brief Semaphore werden zur Synchronisation von Threads verwendet.
 *
 *  Die Klasse Semaphore implementiert das Synchronisationskonzept des zählenden
 *  Semaphors. Die benötigte Warteliste erbt sie dabei von ihrer Basisklasse
 *  Waitingroom.
 */
class Semaphore : public Waitingroom
{
public:
    Semaphore(int c) : counter_(c) {}
    ~Semaphore() {}

    void v();
    void p();

private:
    Semaphore(const Semaphore &copy); // Verhindere Kopieren

    int counter_;

    Waitingroom *waiting_;
};

#endif
