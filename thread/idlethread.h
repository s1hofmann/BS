// vim: set et ts=4 sw=4:

#ifndef __idle_thread_h__
#define __idle_thread_h__

/*! \file
 *  \brief Enthält die Klasse IdleThread
 */

#include "thread/thread.h"

/*! \brief Thread, der immer dann läuft, wenn eine CPU nichts zu tun hat.
 *
 *  Die Verwendung von IdleThread vereinfacht die
 *  Behandlung von "däumchendrehenden" Prozessoren.
 *  \note
 *  Instanzen von IdleThread sollten nie in der Bereitliste des Schedulers
 *  auftauchen, sondern immer getrennt gehalten werden, da sie ja nur dann
 *  ausgeführt werden sollen, wenn kein normaler Thread mehr bereit ist.
 *
 */
class IdleThread : public Thread
{
public:
    IdleThread();
    ~IdleThread();

    void setID(int id) { this->id = id; }
    int getID() { return id; }
    void action();
private:
    IdleThread(const IdleThread &copy); // Verhindere Kopieren
    int id;
    char runstack[4000];
};

#endif // __idle_thread_h__
