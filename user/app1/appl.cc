
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

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

extern Panic panic;
extern Keyboard keyboard;
extern int j;

Application::Application()
{
}

Application::~Application()
{
}

void Application::action ()
{
    int id = system.getCPUID();
    for(long i=0; ; ++i)
    {
        Secure section;
        
        ++j;

        kout.setpos(5,4+id);
        kout << j << endl;
        kout.setpos(2,8);
        kout << "Abgabe" << endl;
        kout.setpos(20, 2);
        kout << "Aufgabe3" << " BS WS14/15" << endl;
        kout.setpos(0, 9);
    }
}
