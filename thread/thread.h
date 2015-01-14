// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Thread
 */
#ifndef __thread_include__
#define __thread_include__

#include "machine/toc.h"
#include "thread/thread.h"
#include "object/queueentry.h"

//Forward declaration
void kickoff(Thread *thread);

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

    virtual int getID() = 0;

private:
    bool kill_flag;
    //toc Struktur zum sichern
    toc registers;
    void *tos_;
};

#endif
