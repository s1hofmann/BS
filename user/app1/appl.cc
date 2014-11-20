
// vim: set et ts=4 sw=4:

#define DEBUG

#include "user/app1/appl.h"
#include "machine/spinlock.h"
#include "device/cgastr.h"
#include "device/panic.h"
#include "device/keyboard.h"
#include "machine/ioapic.h"
#include "machine/cpu.h"
#include "machine/spinlock.h"
#include "object/debug.h"

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

extern Panic panic;
extern Keyboard keyboard;
extern int j;

extern Spinlock global;

Application::Application()
{
}

Application::~Application()
{
}

void Application::action ()
{
    for(long i=0; ; ++i)
    {
        CPU::disable_int();
        global.lock();
        DBG << "Lock enabled, interrupts disabled" << endl;
        //Poor mans guide to modulo
        //if(!(i-((i/100)*100)))
        //{
            ++j;
        //}
        kout.setpos(5,5);
        kout << j << endl;
        kout << j*2 << endl;
        kout << j*4 << endl;
        kout.setpos(2,8);
        kout << "Abgabe" << endl;
        kout.setpos(20, 2);
        kout << "Aufgabe2" << " BS WS14/15" << endl;
        global.unlock();
        CPU::enable_int();
    }
}
