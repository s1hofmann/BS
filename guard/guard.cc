
// vim: set et ts=4 sw=4:

#define DEBUG

#include "object/debug.h"
#include "guard.h"
#include "machine/cpu.h"

void Guard::enter()
{
    //Wait for lock
    while(!avail())
    {
    }
    Locker::enter();
    guard_lock.lock();
}

void Guard::leave()
{
    guard_lock.unlock();
    Locker::retne();
    //Process epilogue_queue
    CPU::disable_int();
    while(Gate *next = epilogues[system.getCPUID()].dequeue())
    {
        //An epilogoue should be interruptable
        CPU::enable_int();
        next->epilogue();
        next->set_dequeued();
        CPU::disable_int();
    }
    CPU::enable_int();
}

void Guard::relay(Gate *item)
{
    queue_lock.lock();
    if(item->set_queued())
    {
        epilogues[system.getCPUID()].enqueue(item);
    }
    queue_lock.unlock();
}
