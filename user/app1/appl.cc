
// vim: set et ts=4 sw=4:

#define DEBUG

#include "user/app1/appl.h"

#include "device/cgastr.h"
#include "device/panic.h"
#include "device/keyboard.h"

#include "guard/guard.h"
#include "guard/secure.h"

#include "machine/ioapic.h"
#include "machine/cpu.h"
#include "machine/spinlock.h"

#include "object/debug.h"

#include "thread/scheduler.h"
#include "syscall/guarded_scheduler.h"
#include "syscall/guarded_semaphore.h"

#define MAIN_WIDTH 79
#define MAIN_HEIGHT 12

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

extern Panic panic;
extern Keyboard keyboard;
extern Scheduler scheduler;

extern Guard guard;

extern Guarded_Semaphore cgaSemaphore;

//Jede Application startet als Thread mit eigenem Stack
//runstack+4000 ergibt die oberste Adresse des Stacks
Application::Application() : Thread(runstack+4000)
{
}

Application::~Application()
{
}

unsigned long Application::rand(void)
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

void Application::action ()
{
    while(true)
    {
        bell.set(10000);

        cgaSemaphore.p();
        CGA_Screen::color colors[3] = {CGA_Screen::DARK_GREY, CGA_Screen::LIGHT_GREEN, CGA_Screen::GREEN};
        unsigned int x = rand()%MAIN_WIDTH;
        unsigned int y = rand()%MAIN_HEIGHT+1;

        for(unsigned int i=1; i<MAIN_HEIGHT; ++i)
        {
            unsigned char textcolor = CGA_Screen::attribute(CGA_Screen::BLACK, colors[i%3], false);
            kout.setcolor(textcolor);
            kout.setpos(x, i);
            char c = rand()%26+'a';
            kout << c << endl;
        }
        kout.setpos(x,y);
        kout.setcolor(CGA_Screen::attribute(CGA_Screen::BLACK, CGA_Screen::RED, false));
        kout << id << endl;
        cgaSemaphore.v();
        
        bell.sleep();
    }
}
