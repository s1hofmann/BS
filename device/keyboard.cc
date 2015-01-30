#define DEBUG

#include "object/debug.h"
#include "machine/cpu.h"
#include "machine/spinlock.h"
#include "keyboard.h"
#include "guard/secure.h"
#include "thread/scheduler.h"

extern IOAPIC ioapic;
extern Plugbox plugbox;
extern CGA_Stream kout;
extern APICSystem system;
extern Scheduler scheduler;

Keyboard::Keyboard() : Gate(), s(0)
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
    Key localk = this->key_hit();
    bool valid = localk.valid();

    if(valid and !k.valid())
    {
        this->k = localk;
    }

    //If input is valid we request an epilogue to display the input
    return valid;
}

//Sollte zwischen Prolog und Epilog ausgeführt werden...
//Aber deshalb stehts nicht extra dazwischen
Key Keyboard::getkey()
{
    //Erstmal blockieren
    s.p();
    
    //Eventuell rebooten
    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        this->reboot();
    }

    //Ansonsten zurückggeben
    Key returnKey = k;
    k.invalidate();

    DBG << "KEY!" << endl;
    return returnKey;
}

void Keyboard::epilogue()
{
    //Im Epilog sollte die ganze Verarbeitung passiert sein, also wird die Semaphore wieder freigegeben
    s.v();
}
