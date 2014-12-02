
// vim: set et ts=4 sw=4:

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
        }
    }
}
