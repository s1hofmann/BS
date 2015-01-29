#define DEBUG
#include "thread/idlethread.h"
#include "machine/apicsystem.h"
#include "object/debug.h"
#include "machine/cpu.h"

extern APICSystem system;

IdleThread::IdleThread() : Thread(runstack+4000)
{

}


IdleThread::~IdleThread()
{

}

void IdleThread::action()
{
    DBG << "Idle " << system.getCPUID() << endl;
    CPU::idle();
    // muss ersetzt werden durch CPU::idle(), wenn wir den IPI zum Wecken implementiert haben
/*    while(true){
        int c = 0;
        for(int i = 0; i<100000; i++){
            for(int j = 0; j<1000; j++){
                c++;
            }
            c -= 1000;
        }
       // DBG << "Still idle..." << endl;
    }
*/
}
