
// vim: set et ts=4 sw=4:

#include "user/app2/kappl.h"
#include "syscall/guarded_semaphore.h"
#include "syscall/guarded_keyboard.h"
#include "device/cgastr.h"

extern Guarded_Semaphore cgaSemaphore;
extern Guarded_Keyboard keyboard;
extern CGA_Stream kout;

KeyboardApplication::KeyboardApplication() : Thread(runstack+4000)
{
}

KeyboardApplication::~KeyboardApplication() {}

void KeyboardApplication::action()
{
    Key k;

    k = keyboard.getkey();

    cgaSemaphore.p();
    kout << k.ascii() << endl;
    cgaSemaphore.v();
}
