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

bool Panic::prologue(){
    kout << "Error!" << endl;
    CPU::halt();
}

void Panic::epilogue(){

}
/*
void Panic::trigger()
{
    kout << "Error!" << endl;
    CPU::halt();
}
*/
