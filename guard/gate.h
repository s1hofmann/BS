#ifndef GATE_H_E0VAPSZB
#define GATE_H_E0VAPSZB
#include "object/queueentry.h"

class Gate : public QueueEntry<Gate>{

<<<<<<< HEAD
#include "object/queueentry.h"

class Gate : public QueueEntry<Gate>
{
public:
    Gate() : queue_flag(false) {};
    ~Gate() {};
    virtual bool prologue() = 0;
    virtual void epilogue() { };
    
    bool set_queued();
    void set_dequeued();

private:
    /* data */

    //Indicates if whether an object has already been enqueued in epilogue queue or not.
    bool queue_flag;
=======
public:
    Gate() {};
    virtual ~Gate() {};
    virtual bool prologue()=0;
    virtual void epilogue() {};
    bool set_queued(){
        return !(__sync_lock_test_and_set(&this->queued, true));
    }
    void set_dequeued(){
        __sync_lock_release(&this->queued);
    }
private:
    bool queued = false;
>>>>>>> testing_marcel_a3
};

#endif /* end of include guard: GATE_H_E0VAPSZB */
