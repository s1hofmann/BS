// vim: set et ts=4 sw=4
#include "device/keyboard.h"
#include "machine/ioapic.h"
#define DEBUG 1
#include "object/debug.h"

// global object definiton
Keyboard keyboard;

extern Plugbox plugbox;
extern IOAPIC ioapic;
extern APICSystem system;
extern CGA_Stream kout;

Keyboard::Keyboard(){
}
void Keyboard::plugin(){
    DBG << "kbd plugin" << endl;
    plugbox.assign(plugbox.keyboard, this);
    ioapic.config(system.getIOAPICSlot(system.keyboard), plugbox.keyboard);
    // shall we call ioapic.allow(system.getIOAPICSlot(system.keyboard)) here?
    ioapic.allow(system.getIOAPICSlot(system.keyboard));
}
void Keyboard::trigger(){
    DBG << "kbd trigger" << endl;
    Key k;
    do{
        k = this->key_hit();
    } while(!k.valid());

    if(k.ctrl() && k.alt() && (k.scancode() == 0x53)){
        DBG << "reboot" << endl;
        this->reboot();
    } else {
        kout.show(20, 0, k.ascii());
    }
}