#define DEBUG
#define MAIN_WIDTH 79

#include "object/debug.h"
#include "machine/cpu.h"
#include "keyboard.h"
#include "machine/spinlock.h"

extern IOAPIC ioapic;
extern Plugbox plugbox;
extern CGA_Stream kout;
extern APICSystem system;
extern Spinlock global;

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

void Keyboard::epilogue() {

//    DBG << "epilogue()" << endl;

    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        this->reboot();
    }

    kout.setpos(posX,0);
    kout << this->k.ascii();
    k.invalidate();
    kout.flush();
    ++posX;
    posX=posX%MAIN_WIDTH;
    if(posX == 0){ kout.cls(' '); }
}

bool Keyboard::prologue() {

 //   DBG << "prologue()" << endl;

    Key localk = this->key_hit();

    bool valid = localk.valid();

    if(valid && !k.valid()){ // k.valid semantics: value consumed, producer go on
        this->k = localk;
    }

    return valid;
}
/*
void Keyboard::trigger()
{
    DBG << "trigger()" << endl;
    
    k = this->key_hit();

    if(k.ctrl() and k.alt() and k.scancode() == Key::scan::del)
    {
        this->reboot();
    }

    if(k.valid())
    {
        kout.setpos(posX,0);
        kout << k.ascii();
        kout.flush();
        ++posX;
        posX=posX%MAIN_WIDTH;
    }
}
*/
