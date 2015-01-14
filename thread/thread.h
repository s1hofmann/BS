// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Thread
 */
#include "machine/toc.h"
#include "object/queueentry.h"

#ifndef __thread_include__
#define __thread_include__



class Thread : public QueueEntry<Thread> {
public:
    // constructor
    Thread(void *tos);

    // activates first thread on a cpu
    void go();

    // switch from current thread to next thread
    void resume(Thread *next);

    // method to be called "as thread"
    virtual void action() = 0;

    // set kill flag, thread tansits to dying-state
    void set_kill_flag();

    // reset kill flag, called when thread is killed
    void reset_kill_flag();

    // query kill flag, indicates if thread is currently dying
    bool dying();
private:
    struct toc regs;
};

void kickoff(Thread *thread){
    thread->action();
}

#endif
