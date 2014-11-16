#ifndef SPINLOCK_H_XFEJG27T
#define SPINLOCK_H_XFEJG27T

class Spinlock
{
public:
    Spinlock() { __sync_lock_release(&the_lock); };
    ~Spinlock() {};

    void lock() { __sync_lock_test_and_set(&the_lock, 1); };
    void unlock() { __sync_lock_release(&the_lock); the_lock = 0; };


private:
    /* data */
    volatile int the_lock = 0;
};

#endif /* end of include guard: SPINLOCK_H_XFEJG27T */
