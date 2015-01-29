// vim: set et ts=4 sw=4:

#ifndef __Guarded_Bell_include__
#define __Guarded_Bell_include__

#include "meeting/bell.h"
#include "guard/secure.h"

/*! \file
 *  \brief Enthält die Klasse Guarded_Bell
 */

/*! \brief Schnittstelle von Anwendungsthreads zu Bell-Objekten.
 *
 *  Die Klasse Guarded_Bell implementiert die Systemaufrufschnittstelle zur
 *  Bell Klasse. Die von Guarded_Bell angebotenen Methoden werden direkt auf
 *  die Methoden der Basisklasse abgebildet, nur dass ihre Ausführung jeweils
 *  mit Hilfe eines Objekts der Klasse Secure geschützt wird.
 */
class Guarded_Bell : public Bell
{
private:
    Guarded_Bell(const Guarded_Bell &copy); // Verhindere Kopieren
public:
    Guarded_Bell() {};
    ~Guarded_Bell() {};
    void sleep() { Secure section; Bell::sleep(); }
    void set(int ms) { Secure section; Bell::set(ms); }
};

#endif
