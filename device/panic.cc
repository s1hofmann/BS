#define DEBUG
#include "object/debug.h"
#include "panic.h"
#include "device/cgastr.h"
#include "machine/cpu.h"

bool Panic::prologue(){

<<<<<<< HEAD
Panic::Panic () : Gate()
{
}

Panic::~Panic()
{
}

bool Panic::prologue()
{
    kout << "Error!" << endl;
    CPU::halt();
    
    return false;
}

void Panic::epilogue()
{
=======
    DBG << "Error!" << endl;
    CPU::halt();

    return false;
>>>>>>> testing_marcel_a3
}
