#define DEBUG
#include "thread/idlethread.h"

#include "object/debug.h"

IdleThread::IdleThread() : Thread(runstack+4000)
{

}


IdleThread::~IdleThread()
{

}

void IdleThread::action()
{
    DBG << "Idle!" << endl;
    // muss ersetzt werden durch CPU::idle(), wenn wir den IPI zum Wecken implementiert haben
    while(1){
        int c = 0;
        for(int i = 0; i<100000; i++){
            for(int j = 0; j<1000; j++){
                c++;
            }
            c -= 1000;
        }

        DBG << "Still idle..." << endl;
    }
}
