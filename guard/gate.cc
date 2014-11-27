#include "gate.h"

//Sets the enqueued flag.
//Returns false if flag has already been set, true if not.
bool Gate::set_queued()
{
    bool flag = __sync_lock_test_and_set(&queue_flag, true);

    return flag ? false : true;
}

void Gate::set_dequeued()
{
    __sync_lock_release(&queue_flag);
}
