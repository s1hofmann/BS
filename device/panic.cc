#include "panic.h"
#include "device/cgastr.h"
#include "machine/cpu.h"

extern CGA_Stream kout;

Panic::Panic () : Gate()
{
}

Panic::~Panic()
{
}

void Panic::trigger()
{
    kout << "Error!" << endl;
    CPU::halt();
}
