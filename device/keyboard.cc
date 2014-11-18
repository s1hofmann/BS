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
    do
    {
        k = kc.key_hit();
        if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
        {
            kc.reboot();
        }
    } while(!(k.valid()));
    kout << k.ascii();
    kout.flush();
}
