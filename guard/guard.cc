
// vim: set et ts=4 sw=4:

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
    this->Locker::enter();
    this->guardlock.lock();
    // wait actively
    // while(!this->avail()){}; // according to tim&cuong not needed
}

void Guard::leave(){
    //DBG << "leave" << endl;

    // check queue
    CPU::disable_int();
    while(Gate* g = this->queue[system.getCPUID()].dequeue()){
        CPU::enable_int();
        g->epilogue();
        CPU::disable_int();
        g->set_dequeued();
    }
    CPU::enable_int();

    this->Locker::retne();
    this->guardlock.unlock();
}

void Guard::relay(Gate *item){
    //DBG << "relay" << endl;
    if(avail()){
        this->Locker::enter();
        this->guardlock.lock();

        CPU::enable_int();
        item->epilogue();

        this->guardlock.unlock();
        this->Locker::retne();

    } else { //assert 0 <= CPUID < 4
        if (item->set_queued()){
            this->queue[system.getCPUID()].enqueue(item);
        } else {
            DBG << "lost" << endl;
        }
    }
}
