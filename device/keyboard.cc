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
    drainKeyboardBuffer();
}

void Keyboard::trigger()
{
    k = kc.key_hit();
    if(k.valid())
    {
        kout << k.ascii();
        kout.flush();
    }
}
