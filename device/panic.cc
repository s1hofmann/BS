#define DEBUG
#include "object/debug.h"
#include "panic.h"
#include "device/cgastr.h"
#include "machine/cpu.h"

bool Panic::prologue(){

    DBG << "Error!" << endl;
    CPU::halt();

    return false;
}
