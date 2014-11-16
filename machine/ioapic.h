#ifndef IOAPIC_H
#define IOAPIC_H 

#include "types.h" 
#include "machine/ioapic_register.h"
#include "machine/plugbox.h"
#include "device/cgastr.h"

class IOAPIC
{
public:
    IOAPIC ();
    virtual ~IOAPIC ();

    //Initializes IOAPIC
    void init();
    //Maps a vector from IVT to an external interrupt
    void config(unsigned char slot, Plugbox::Vector vector);
    //Enables passing of interrupts to CPU. In order to enable interrupt handling CPU::enable_int() has to be called.
    void allow(unsigned char slot);
    //Lock selected interrupts.
    void forbid(unsigned char slot);
    //Returns interrupt status
    bool status(unsigned char slot);

    //Memory-mapped IO-APIC register in CPU address space
    static volatile uint32_t* IOREGSEL_REG;
    static volatile uint32_t* IOWIN_REG;

private:
    /* data */
};
#endif
