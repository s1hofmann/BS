
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
#include "guard/guard.h"
#include "guard/secure.h"

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

extern Panic panic;
extern Keyboard keyboard;
extern Guard globalGuard;

<<<<<<< HEAD
=======
extern int j;

>>>>>>> testing_marcel_a3
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
<<<<<<< HEAD
        Secure section;
        
        ++j;
=======
	Secure s;
        //DBG << "Lock enabled, interrupts disabled" << endl;
        //Poor mans guide to modulo
        //if(!(i-((i/100)*100)))
        //{
            ++j;
        //}
>>>>>>> testing_marcel_a3

        kout.setpos(5,4+id);
        kout << j << endl;

        kout.setpos(2,8);
        kout << "Abgabe" << endl;

        kout.setpos(20, 2);
<<<<<<< HEAD
        kout << "Aufgabe3" << " BS WS14/15" << endl;
        kout.setpos(0, 9);
=======
        kout << "Aufgabe2" << " BS WS14/15" << endl;

>>>>>>> testing_marcel_a3
    }
}
