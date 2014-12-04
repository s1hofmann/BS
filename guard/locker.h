<<<<<<< HEAD
=======

>>>>>>> testing_marcel_a3
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Diese Datei enthält die Klasse Locker.
 */

#ifndef __Locker_include__
#define __Locker_include__

<<<<<<< HEAD
#include "machine/apicsystem.h"

=======

#include "machine/apicsystem.h"
#include "types.h"

extern APICSystem system;
>>>>>>> testing_marcel_a3
/*! \brief Die Klasse Locker dient dem Schutz kritischer Abschnitte.
 *
 *  Dazu verwaltet sie eine Sperrvariable für jeden Prozessor, die angibt, ob
 *  der jeweilige Prozessor sich gerade im kritische Abschnitt befindet.
 *
 *  Die Klasse Locker bestimmt jedoch nicht, was zu tun ist, wenn der kritische
 *  Abschnitt besetzt ist. Ebenso trifft sie keine Vorkehrungen, um ihre
 *  eigenen kritischen Abschnitte zu schützen.
 */
<<<<<<< HEAD

extern APICSystem system;

=======
>>>>>>> testing_marcel_a3
class Locker
{
private:
    Locker(const Locker &copy); // Verhindere Kopieren
<<<<<<< HEAD

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

=======
    uint32_t lock; // enough space for 32 CPUs
    bool blocked[4];
public:
    /*! \brief Konstruktor: Initialisiert die Sperrvariable so, dass der
     *  kritische Abschnitt als frei markiert wird.
     */
     Locker(){
        blocked[0] = false;
        blocked[1] = false;
        blocked[2] = false;
        blocked[3] = false;
     }

    /*! \brief Diese Methode muss aufgerufen werden, wenn der kritische
     *  Abschnitt betreten wird.
     */
    void enter() {
        // assert CPUID >= 0
        // assert CPUID <= 31
        // this is going to break if CPUID < 0
        // this->lock ^= 1 << system.getCPUID();
        blocked[system.getCPUID()] = true;
    }
    /*! \brief Mit dieser Methode wird der kritische Abschnitt wieder verlassen.
     */
    void retne() {
        // assert CPUID >= 0
        // assert CPUID <= 31
        // this is going to break if CPUID < 0
        //this->lock ^= 1 << system.getCPUID();
        blocked[system.getCPUID()] = false;
    }
>>>>>>> testing_marcel_a3
    /*! \brief Diese Methode gibt an, ob der kritische Abschnitt frei ist.
     *  \return Gibt \b true zurück, falls der kritische Abschnitt frei ist,
     *  ansonsten \b false.
     */
<<<<<<< HEAD
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
=======
    bool avail() const {
        // TODO: state accross cpus or just for the current one?
        //return !(this->lock);
        return !blocked[system.getCPUID()];
    }
    
>>>>>>> testing_marcel_a3
};

#endif
