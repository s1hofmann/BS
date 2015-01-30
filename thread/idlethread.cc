#define DEBUG
#include "thread/idlethread.h"
#include "machine/cpu.h"
#include "object/debug.h"
#include "thread/scheduler.h"
#include "syscall/guarded_scheduler.h"
#include "machine/cpu.h"

extern Scheduler scheduler;

IdleThread::IdleThread() : Thread(runstack+4000)
{

}

IdleThread::~IdleThread()
{

}

void IdleThread::action()
{
    while(true)
    {
        //Interrupts disablen
        CPU::disable_int();
        if(scheduler.isEmpty())
        {
            DBG << "IDLE" << endl;
            CPU::idle();
        }
        else
        {
            Guarded_Scheduler::resume();
        }
    }
}
