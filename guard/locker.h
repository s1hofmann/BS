// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Diese Datei enthält die Klasse Locker.
 */

#ifndef __Locker_include__
#define __Locker_include__

#include "machine/apicsystem.h"

/*! \brief Die Klasse Locker dient dem Schutz kritischer Abschnitte.
 *
 *  Dazu verwaltet sie eine Sperrvariable für jeden Prozessor, die angibt, ob
 *  der jeweilige Prozessor sich gerade im kritische Abschnitt befindet.
 *
 *  Die Klasse Locker bestimmt jedoch nicht, was zu tun ist, wenn der kritische
 *  Abschnitt besetzt ist. Ebenso trifft sie keine Vorkehrungen, um ihre
 *  eigenen kritischen Abschnitte zu schützen.
 */

extern APICSystem system;

class Locker
{
private:
    Locker(const Locker &copy); // Verhindere Kopieren

    //Same as in Guard, hardcoding the number of locks is not cool!
    char locks[CPU_MAX];
     
public:
    enum state {locked=1, unlocked=0};

    /*! \brief Konstruktor: Initialisiert die Sperrvariable so, dass der
     *  kritische Abschnitt als frei markiert wird.
     */
    Locker()
    {
        for(int i=0; i<CPU_MAX; ++i)
        {
            locks[i] = 0;
        }
    };

    ~Locker() {};
     
    /*! \brief Diese Methode muss aufgerufen werden, wenn der kritische
     *  Abschnitt betreten wird.
     */
    void enter() {locks[system.getCPUID()] = locked;}
    
    /*! \brief Mit dieser Methode wird der kritische Abschnitt wieder verlassen.
     */
    void retne() {locks[system.getCPUID()] = unlocked;}

    /*! \brief Diese Methode gibt an, ob der kritische Abschnitt frei ist.
     *  \return Gibt \b true zurück, falls der kritische Abschnitt frei ist,
     *  ansonsten \b false.
     */
    bool avail() const
    {
        for(int i=0; i<CPU_MAX; ++i)
        {
            //Another CPU has already entered the critical section
            if(locks[i] == locked)
            {
                return false;
            }
        }
        return true;
    }
};

#endif
