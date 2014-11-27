#ifndef LOCKER_H_IIMX7T0W
#define LOCKER_H_IIMX7T0W

class Locker
{
public:
    Locker() : the_lock(0);
    ~Locker();

    void enter() { while(__sync_lock_test_and_set(&the_lock, 1)){}; };
    void retne() { __sync_lock_release(&the_lock); };
    bool avail() { the_lock ? return false : return true; }

private:
    /* data */
    volatile int the_lock;
};

#endif /* end of include guard: LOCKER_H_IIMX7T0W */
