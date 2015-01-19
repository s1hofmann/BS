// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Thread
 */
#ifndef __thread_include__
#define __thread_include__

#include "machine/toc.h"
#include "thread/thread.h"
#include "object/queueentry.h"
#include "meeting/waitingroom.h"

//Forward declaration
void kickoff(Thread *thread);
class Waitingroom;

class Thread : public QueueEntry<Thread>
{
public:
    Thread(void *tos);
    virtual void action() = 0;
    void go();
    void resume(Thread *next);

    void set_kill_flag() { kill_flag=true; }
    void reset_kill_flag() { kill_flag=false; }
    bool dying() { return kill_flag; }
    Waitingroom *waiting_in() { return myWaitingroom_; }
    void waiting_in(Waitingroom *w) { myWaitingroom_ = w; }

    virtual int getID() = 0;

    Waitingroom *myWaitingroom_ = 0;

private:
    bool kill_flag;
    //toc Struktur zum sichern
    toc registers;
    void *tos_;
};

#endif
