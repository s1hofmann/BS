// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Guarded_Scheduler
 */

#ifndef __guarded_scheduler_include__
#define __guarded_scheduler_include__

#include "thread/scheduler.h"
#include "guard/secure.h"

extern Scheduler scheduler;

/*! \brief Systemaufrufschnittstelle zum Scheduler
 *
 *  Der Guarded_Scheduler implementiert die Systemaufrufschnittstelle zum
 *  Scheduler. Die von ihm angebotenen Methoden werden direkt auf die Methoden
 *  des globalen Scheduler-Objekts abgebildet, nur dass ihre Ausführung jeweils
 *  mit Hilfe eines Objekts der Klasse Secure geschützt wird.
 */

class Guarded_Scheduler
{
public:
    static void	ready(Thread *that)
    {
        Secure section;
        scheduler.ready(that);
    }

    static void exit()
    {
        Secure section;
        scheduler.exit();
    }

    static void kill(Thread *that)
    {
        Secure section;
        scheduler.kill(that);
    }

    static void resume()
    {
        Secure section;
        scheduler.resume();
    }
};

#endif
