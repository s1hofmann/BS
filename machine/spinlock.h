#ifndef SPINLOCK_H_XFEJG27T
#define SPINLOCK_H_XFEJG27T

class Spinlock
{
public:
    Spinlock() { __sync_lock_release(&the_lock); };
    ~Spinlock() {};

    /*
     *__sync_lock_test_and_set tries to write the second parameter to *ptr and returns its previous value.
     *So if it returns 1, it means that some other thread has already locked the section, so we'll wait in a while loop until the value is reset.
     *Unlocking usually means setting the value of *ptr to 0.
     */
    void lock() { while(__sync_lock_test_and_set(&the_lock, 1)){}; };
    void unlock() { __sync_lock_release(&the_lock); };

private:
    /* data */
    volatile int the_lock;
};

#endif /* end of include guard: SPINLOCK_H_XFEJG27T */
