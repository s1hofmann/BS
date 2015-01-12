// vim: set et ts=4 sw=4:

#ifndef __dispatch_include__
#define __dispatch_include__

#include "machine/apicsystem.h"
#include "thread/thread.h"
#include "machine/spinlock.h"

extern APICSystem system;

/*! \file
 *
 *  \brief Enthält die Klasse Dispatcher.
 */

/*! \brief Implementierung des Dispatchers
 *
 *  Der Dispatcher verwaltet den Life-Pointer, der die jeweils aktive Koroutine
 *  angibt und führt die eigentlichen Prozesswechsel durch. In der
 *  Uniprozessorvariante wird nur ein einziger Life-Pointer benötigt, da
 *  lediglich ein Prozess auf einmal aktiv sein kann. Für die
 *  Mehrprozessorvariante wird hingegen für jede CPU ein eigener Life-Pointer
 *  benötigt.
 *
 */
class Dispatcher
{
public:
    Dispatcher() {}

    Thread *active() { return life[system.getCPUID()]; }
    void go(Thread *first);
    void dispatch(Thread *then);

private:
    Dispatcher(const Dispatcher &copy); // Verhindere Kopieren
    void setActive(Thread *c);
    Thread *life[CPU_MAX];
};

#endif
