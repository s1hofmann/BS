#define DEBUG
#define MAIN_WIDTH 79

#include "object/debug.h"
#include "machine/cpu.h"
#include "keyboard.h"
#include "machine/spinlock.h"
#include "guard/secure.h"

extern IOAPIC ioapic;
extern Plugbox plugbox;
extern CGA_Stream kout;
extern APICSystem system;
extern Spinlock global;

extern int posX, j;

Keyboard::Keyboard() : Gate()
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

bool Keyboard::prologue()
{
    //DBG << "KBD prologue()" << endl;
    
    k = this->key_hit();

    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        this->reboot();
    }

    //If input is valid we request an epilogue to display the input
    return true;
}

void Keyboard::epilogue()
{
    //DBG << "KBD epilogue()" << endl;
    if(k.valid())
    {
        kout.setpos(posX,0);
        kout << k.ascii();
        kout.flush();
        ++posX;
        posX=posX%MAIN_WIDTH;
    }
}
