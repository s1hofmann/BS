
// vim: set et ts=4 sw=4:

#define DEBUG

#include "object/debug.h"
#include "guard.h"
#include "machine/cpu.h"

void Guard::enter()
{
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
    while(Gate *next = epilogues[system.getCPUID()].dequeue())
    {
        //An epilogoue should be interruptable
        CPU::enable_int();
        next->set_dequeued();
        next->epilogue();
        CPU::disable_int();
    }
    CPU::enable_int();
    guard_lock.unlock();
    Locker::retne();
}

void Guard::relay(Gate *item)
{
    if(avail())
    {
        Locker::enter();
        guard_lock.lock();
        item->epilogue();
        guard_lock.unlock();
        Locker::retne();
    }
    else
    {
        if(item->set_queued())
        {
            CPU::disable_int();
            epilogues[system.getCPUID()].enqueue(item);
            CPU::enable_int();
        }
    }
}
