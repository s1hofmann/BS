#define DEBUG
#include "thread/idlethread.h"
#include "machine/cpu.h"
#include "object/debug.h"
#include "syscall/guarded_scheduler.h"

IdleThread::IdleThread() : Thread(runstack+4000)
{

}


IdleThread::~IdleThread()
{

}

void IdleThread::action()
{
    DBG << "IDLE" << endl;
    CPU::idle();
    Guarded_Scheduler::resume();
}
