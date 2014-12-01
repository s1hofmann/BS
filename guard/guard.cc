
// vim: set et ts=4 sw=4:

#include "guard/guard.h"
#include "machine/apicsystem.h"
#include "machine/spinlock.h"

// remember to lock critical sections against other cpus
Spinlock guardlock;
extern APICSystem system;

void Guard::enter(){
    guardlock.lock();
    // wait actively
    while(!this->avail()){};
    this->Locker::enter();
    guardlock.unlock();
}

void Guard::leave(){
    guardlock.lock();
    this->Locker::retne();
    guardlock.unlock();
}

void Guard::relay(Gate *item){
    guardlock.lock();
    //assert 0 <= CPUID < 4
    this->queue[system.getCPUID()].enqueue(item);
    guardlock.unlock();
}
