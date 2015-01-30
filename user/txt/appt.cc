
// vim: set et ts=4 sw=4:

#define DEBUG

#include "user/txt/appt.h"

#include "device/cgastr.h"
#include "device/panic.h"
#include "device/keyboard.h"

#include "guard/guard.h"
#include "guard/secure.h"

#include "machine/ioapic.h"
#include "machine/cpu.h"
#include "machine/spinlock.h"

#include "thread/scheduler.h"
#include "syscall/guarded_scheduler.h"
#include "syscall/guarded_semaphore.h"

#include "object/debug.h"

#define MAIN_WIDTH 79
#define MAIN_HEIGHT 12

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

extern Guarded_Semaphore cgaSemaphore;

//Jede TxtApp startet als Thread mit eigenem Stack
//runstack+4000 ergibt die oberste Adresse des Stacks
TxtApp::TxtApp() : Thread(runstack+4000)
{
}

TxtApp::~TxtApp()
{
}

void TxtApp::action ()
{
    while(true)
    {
        DBG << "TEXT" << endl;
        b.set(500);
        b.sleep();

        cgaSemaphore.p();
        for(int i=0; i<1000; ++i)
        {
            kout.setpos(0,0);
            kout.setcolor(CGA_Screen::attribute(CGA_Screen::BLACK, CGA_Screen::WHITE, false));
            kout << "knock, knock..." << endl << endl << "follow the white rabbit..." << endl;
        }
        cgaSemaphore.v();
        Guarded_Scheduler::resume();
    }
}
