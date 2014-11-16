
// vim: set et ts=4 sw=4:

#include "user/app1/appl.h"
#include "machine/spinlock.h"
#include "device/cgastr.h"

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

Application::Application()
{
}

Application::~Application()
{
}

void Application::action ()
{
    kout << "hallo!" << endl;
    dout_CPU0 << "test" << endl;
    dout_CPU1 << "hurrdurr" << endl;
     
}
