#define DEBUG

#include "object/debug.h"
#include "keyboard.h"
#include "../machine/spinlock.h"

extern IOAPIC ioapic;
extern Plugbox plugbox;
extern CGA_Stream kout;
extern APICSystem system;

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
    DBG << "trigger()";
    do
    {
        k = kc.key_hit();
    } while(!(k.valid()));

    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        kc.reboot();
    }

    kout << k.ascii();
    kout.flush();
}
