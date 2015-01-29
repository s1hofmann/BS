// vim: set et ts=4 sw=4:

#include "syscall/guarded_keyboard.h"
#include "guard/secure.h"

Key Guarded_Keyboard::getkey()
{
    Secure section;
    return Keyboard::getkey();
}
