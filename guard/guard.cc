
// vim: set et ts=4 sw=4:

#include "guard/guard.h"
#include "machine/cpu.h"
#include "machine/apicsystem.h"
#include "machine/spinlock.h"
#define DEBUG
#include "object/debug.h"

// remember to lock critical sections against other cpus
Guard globalGuard;

extern APICSystem system;

void Guard::enter(){
    DBG << "enter" << endl;
    this->guardlock.lock();
    // wait actively
    while(!this->avail()){};
    this->Locker::enter();
}

void Guard::leave(){
    DBG << "leave" << endl;

    // check queue
    CPU::disable_int();
    while(Gate* g = this->queue[system.getCPUID()].dequeue()){
       g->epilogue();
       g->set_dequeued();
    }
    CPU::enable_int();

    this->Locker::retne();
    this->guardlock.unlock();
}

void Guard::relay(Gate *item){
    DBG << "relay" << endl;
    // TODO: How to start execution directly on E1/2 if free?

    //assert 0 <= CPUID < 4
    if(item->set_queued()){
        this->queue[system.getCPUID()].enqueue(item);
     }
}
