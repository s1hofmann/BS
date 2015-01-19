// vim: set et ts=4 sw=4:

#ifndef __Guarded_Semaphore_include__
#define __Guarded_Semaphore_include__

/*! \file
 *  \brief Enthält die Klasse Guarded_Semaphore
 */

/*! \brief Systemaufrufschnittstelle zum Semaphor
 *
 *  Die Klasse Guarded_Semaphore implementiert die Systemaufrufschnittstelle zur
 *  Semaphore Klasse. Die von Guarded_Semaphore angebotenen Methoden werden
 *  direkt auf die Methoden der Basisklasse abgebildet, nur dass ihre Ausführung
 *  jeweils mit Hilfe eines Objekts der Klasse Secure geschützt wird.
 */
class Guarded_Semaphore
{
private:
    Guarded_Semaphore(const Guarded_Semaphore &copy); // Verhindere Kopieren
};

#endif
