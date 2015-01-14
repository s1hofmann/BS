
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

extern Application apps[];
int counter = 0;

Application::Application(int i) : Thread(this->tos) {
    this->id = i;
}

Application::~Application()
{
}

void Application::action ()
{
    for(long i=0; ; ++i)
    {

        kout.setpos(5,4+this->id);
        kout << i << endl;
        counter++;
        counter = counter % 4;
        this->resume(&apps[counter]);
    }
}
