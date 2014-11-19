#define DEBUG
#define MAIN_WIDTH 79

#include "object/debug.h"
#include "keyboard.h"
#include "machine/spinlock.h"

extern Spinlock global;

extern IOAPIC ioapic;
extern Plugbox plugbox;
extern CGA_Stream kout;
extern APICSystem system;

extern int posX, j;

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::plugin()
{
    plugbox.assign(plugbox.keyboard, this);
    ioapic.config(system.getIOAPICSlot(APICSystem::keyboard), plugbox.keyboard);
    ioapic.allow(system.getIOAPICSlot(APICSystem::keyboard));
    drainKeyboardBuffer();
}

void Keyboard::trigger()
{
    global.lock();
    DBG << "trigger()";
    do
    {
        k = kc.key_hit();
    } while(!(k.valid()));

    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        kc.reboot();
    }

    kout.setpos(posX,0);
    kout << k.ascii();
    kout.flush();
    ++posX;
    posX=posX%MAIN_WIDTH;
    global.unlock();
}
