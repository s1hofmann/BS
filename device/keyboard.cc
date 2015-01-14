#define DEBUG
#define MAIN_WIDTH 79

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
    Key localk = this->key_hit();
    bool valid = localk.valid();

    if(valid and !k.valid())
    {
        this->k = localk;
    }

    //If input is valid we request an epilogue to display the input
    return valid;
}

void Keyboard::epilogue()
{
    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        this->reboot();
    }
    else if(k.ascii()=='k')
    {
        DBG << "KILLING IN THE NAME OF..." << endl << endl;
        scheduler.kill(scheduler.active());
        DBG << "killed thread with id: " << scheduler.active()->getID() << endl << endl;
    }

    kout.setpos(posX,0);
    kout << k.ascii();
    k.invalidate();
    kout.flush();
    ++posX;
    posX=posX%MAIN_WIDTH;

    if(posX==0)
    {
        kout.cls(' ');
    }
}
