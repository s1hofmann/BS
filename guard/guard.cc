
// vim: set et ts=4 sw=4:

<<<<<<< HEAD
#define DEBUG

#include "object/debug.h"
#include "guard.h"
#include "machine/cpu.h"

void Guard::enter()
{
    //CPU enters level .5
    //Critical section, set lock
    guard_lock.lock();
    //Wait for lock
    while(!avail())
    {
    }
    Locker::enter();
}

void Guard::leave()
{
    //Process epilogue_queue
    CPU::disable_int();
    //Before leaving level .5 process epilogue queue
    while(Gate *next = epilogues[system.getCPUID()].dequeue())
    {
        //An epilogue should be interruptable
        CPU::enable_int();
        next->set_dequeued();
        next->epilogue();
        CPU::disable_int();
    }
    Locker::retne();
    guard_lock.unlock();
    CPU::enable_int();
}

void Guard::relay(Gate *item)
{
    //Check if it is possible to switch to level .5 (1->.5)
    //If so, process the current epilogue right away
    if(avail())
    {
        DBG << "relay run" << endl;
        enter();
        CPU::enable_int();
        item->epilogue();
        leave();
    }
    //Otherwise enqueue it
    else
    {
        if(item->set_queued())
        {
            DBG << "relay enqueue" << endl;
            epilogues[system.getCPUID()].enqueue(item);
            CPU::enable_int();
=======
#include "guard/guard.h"
#include "machine/cpu.h"
#include "machine/apicsystem.h"
#include "machine/spinlock.h"
#define DEBUG
#include "object/debug.h"

Guard globalGuard;

extern APICSystem system;

void Guard::enter(){
    //DBG << "enter" << endl;

    /* stupid: note on order: vice-versa potentially 4 cpus are on E1/2 at the same time
       this would block epilogues in irqs from beeing executed directly  */
    /* more important: consider deadlocks! */
    this->Locker::enter();
    this->guardlock.lock();

}

void Guard::leave(){
    //DBG << "leave" << endl;

    // check queue
    CPU::disable_int();
    while(Gate* g = this->queue[system.getCPUID()].dequeue()){
        g->set_dequeued();
        CPU::enable_int();
        g->epilogue();
        CPU::disable_int();
    }


    /* note on order: vice-versa deadlock occurs
     * scenario: cpuX calls retne, cpuX irq occurs, waits for its own lock
     */
    // LOST WAKEUP: dequeu and next two statements are ONE critical section
    this->guardlock.unlock();
    this->Locker::retne();
    CPU::enable_int();
}

void Guard::relay(Gate *item){
    //DBG << "relay" << endl;
    if(this->avail()){ // avail() == queue empty?
        DBG << "epilogue directly" << endl;
        this->Locker::enter();
	// not after lock(): do not wait for locks on E1
        CPU::enable_int();
        this->guardlock.lock();

        item->epilogue();

        this->guardlock.unlock();
        this->Locker::retne();

    } else { //assert 0 <= CPUID < 4
        if (item->set_queued()){
            DBG << "queued" << endl;
            this->queue[system.getCPUID()].enqueue(item);
        } else {
            DBG << "lost" << endl;
>>>>>>> testing_marcel_a3
        }
    }
}
