
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Diese Datei enthält die Klasse Locker.
 */

#ifndef __Locker_include__
#define __Locker_include__


#include "machine/apicsystem.h"
#include "types.h"

extern APICSystem system;
/*! \brief Die Klasse Locker dient dem Schutz kritischer Abschnitte.
 *
 *  Dazu verwaltet sie eine Sperrvariable für jeden Prozessor, die angibt, ob
 *  der jeweilige Prozessor sich gerade im kritische Abschnitt befindet.
 *
 *  Die Klasse Locker bestimmt jedoch nicht, was zu tun ist, wenn der kritische
 *  Abschnitt besetzt ist. Ebenso trifft sie keine Vorkehrungen, um ihre
 *  eigenen kritischen Abschnitte zu schützen.
 */
class Locker
{
private:
    Locker(const Locker &copy); // Verhindere Kopieren
    uint32_t lock; // enough space for 32 CPUs
public:
    /*! \brief Konstruktor: Initialisiert die Sperrvariable so, dass der
     *  kritische Abschnitt als frei markiert wird.
     */
     Locker(){
        this->lock = 0;
    }
    /*! \brief Diese Methode muss aufgerufen werden, wenn der kritische
     *  Abschnitt betreten wird.
     */
    void enter() {
        // assert CPUID >= 0
        // assert CPUID <= 31
        // this is going to break if CPUID < 0
        this->lock ^= 1 << system.getCPUID();
    }
    /*! \brief Mit dieser Methode wird der kritische Abschnitt wieder verlassen.
     */
    void retne() {
        // assert CPUID >= 0
        // assert CPUID <= 31
        // this is going to break if CPUID < 0
        this->lock ^= 1 << system.getCPUID();
    }
    /*! \brief Diese Methode gibt an, ob der kritische Abschnitt frei ist.
     *  \return Gibt \b true zurück, falls der kritische Abschnitt frei ist,
     *  ansonsten \b false.
     */
    bool avail() const {
        return !(this->lock);
    }
    
};

#endif
