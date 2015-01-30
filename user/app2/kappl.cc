
// vim: set et ts=4 sw=4:

#include "user/app2/kappl.h"
#include "syscall/guarded_semaphore.h"
#include "syscall/guarded_keyboard.h"
#include "device/cgastr.h"

#define MAIN_WIDTH 79
#define MAIN_HEIGTH 12

extern Guarded_Semaphore cgaSemaphore;
extern Guarded_Keyboard keyboard;
extern CGA_Stream kout;
extern int posX, j;

KeyboardApplication::KeyboardApplication() : Thread(runstack+4000)
{
}

KeyboardApplication::~KeyboardApplication() {}

void KeyboardApplication::action()
{
    Key k;

    k = keyboard.getkey();

    cgaSemaphore.p();
    kout.setpos(posX,0);
    kout.setcolor(CGA_Screen::attribute(CGA_Screen::BLACK, CGA_Screen::WHITE, false));
    kout << k.ascii();
    kout.flush();
    ++posX;
    posX=posX%MAIN_WIDTH;

    if(posX==0)
    {
        kout.cls(' ');
    }
    cgaSemaphore.v();
}
