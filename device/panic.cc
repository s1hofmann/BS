// vim: set et ts=4 sw=4
#include "panic.h"
#include "machine/cpu.h"
#define DEBUG 1
#include "object/debug.h"

// global object definition
Panic panic;

void Panic::trigger(){
    DBG << "Error: Panic!" << endl;
    CPU::halt();
}
