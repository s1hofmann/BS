
// vim: set et ts=4 sw=4:

#define DEBUG

#include "object/debug.h"
#include "guard.h"
#include "machine/cpu.h"

void Guard::enter()
{
    //CPU enters level .5
    //Critical section, set lock
    Locker::enter();
    guard_lock.lock();
}

void Guard::leave()
{
    //Process epilogue_queue
    CPU::disable_int();
    //Before leaving level .5 process epilogue queue
    while(Gate *next = epilogues[system.getCPUID()].dequeue())
    {
        next->set_dequeued();
        //An epilogue should be interruptable
        CPU::enable_int();
        next->epilogue();
        CPU::disable_int();
    }

    guard_lock.unlock();
    Locker::retne();
    CPU::enable_int();
}

void Guard::relay(Gate *item)
{
    //Check if it is possible to switch to level .5 (1->.5)
    //If so, process the current epilogue right away
    if(avail())
    {
        DBG << "epilogue directly" << endl;
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
        }
    }
}
