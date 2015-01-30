
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

unsigned long TxtApp::rand(void)
{
   static unsigned int z1 = 987654321, z2 = 987654321, z3 = 987654321, z4 = 987654321;
   unsigned int b;
   b  = ((z1 << 6) ^ z1) >> 13;
   z1 = ((z1 & 4294967294U) << 18) ^ b;
   b  = ((z2 << 2) ^ z2) >> 27; 
   z2 = ((z2 & 4294967288U) << 2) ^ b;
   b  = ((z3 << 13) ^ z3) >> 21;
   z3 = ((z3 & 4294967280U) << 7) ^ b;
   b  = ((z4 << 3) ^ z4) >> 12;
   z4 = ((z4 & 4294967168U) << 13) ^ b;
   return (z1 ^ z2 ^ z3 ^ z4);
}

void TxtApp::action ()
{
    DBG << "TEXT" << endl;
    b.set(5000);

    cgaSemaphore.p();

    unsigned int x = rand()%(MAIN_WIDTH-20);
    unsigned int y = rand()%(MAIN_HEIGHT-5)+1;

    for(int i=0; i<1000; ++i)
    {
        kout.setpos(x,y);
        kout.setcolor(CGA_Screen::attribute(CGA_Screen::BLACK, CGA_Screen::WHITE, false));
        kout << "knock, knock..." << endl << endl;
        kout << "follow the white rabbit...";
    }
    cgaSemaphore.v();
    b.sleep();
}
