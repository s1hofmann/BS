// vim: set et ts=4 sw=4
#include "machine/spinlock.h"

// global object definition
Spinlock lock;

Spinlock::Spinlock(){
    this->locked = false;
}
void Spinlock::lock(){
    /* should loop until __sync_lock_test_and_set(type *ptr, type val)
     * returns 0, this should happen initially when locked is 0
     * or when someone called unlock
     */
    while(__sync_lock_test_and_set(&this->locked, 1)) { }
}
void Spinlock::unlock(){
    __sync_lock_release(&this->locked);
}





