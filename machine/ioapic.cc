#include "ioapic.h"
#include "ioapic_register.h"
#include "apicsystem.h"
#include "plugbox.h"

volatile uint32_t *IOAPIC::IOREGSEL_REG = (volatile uint32_t*)0xfec00000;
volatile uint32_t *IOAPIC::IOWIN_REG = (volatile uint32_t*)0xfec00010;

extern APICSystem system;
extern Plugbox plugbox;

IOAPIC::IOAPIC()
{
}

IOAPIC::~IOAPIC()
{
}

void IOAPIC::init()
{
    IOAPICID id;
    IOREDTBL_L low;
    IOREDTBL_H high;

    IOAPICRegister l;
    IOAPICRegister h;
    IOAPICRegister i;

    i.IOAPICID = id;
    l.IOREDTBL_L = low;
    h.IOREDTBL_H = high;

    id.ID = system.getIOAPICID();

    *IOREGSEL_REG = IOAPIC_ID_REG;
    *IOWIN_REG = i.value;

    low.vector = plugbox.keyboard;
    low.delivery_mode = DELIVERY_MODE_LOWESTPRI;
    low.destination_mode = DESTINATION_MODE_LOGICAL;
    low.mask = MASK_ENABLED;
    low.polarity = POLARITY_HIGH;
    low.trigger_mode = TRIGGER_MODE_EDGE;

    high.logical_destination = 0xff;
    high.reserved = 0;

    //Randomly choosing a slot is NOT the best way to init the IOAPIC
    //After reading through the docs I changed it to the appropriate keyboard slot
    //returned by getIOAPICSlot()
    
    //Calculate offset, 64 bit (2 registers) per entry
    unsigned int offset = IOAPIC_REDTBL_REG+system.getIOAPICSlot(APICSystem::keyboard)*2;
    //Write low part
    *IOREGSEL_REG = offset;
    *IOWIN_REG = l.value;
    //Move 32 bits (one register) and write high part
    *IOREGSEL_REG += 1;
    *IOWIN_REG = h.value;
}

void IOAPIC::config(unsigned char slot, Plugbox::Vector vector)
{
    unsigned int offset = IOAPIC_REDTBL_REG+slot*2;
    unsigned int in;
    *IOREGSEL_REG = offset;
    in = *IOWIN_REG;
    //Clear old vector entries
    in &= 0xFFFFFF00;
    //And write new values
    in |= vector;

    *IOWIN_REG = in;
}

void IOAPIC::allow(unsigned char slot)
{
    unsigned int offset = IOAPIC_REDTBL_REG+slot*2;
    *IOREGSEL_REG = offset;
    *IOWIN_REG &= 0xFFFeFFFF;
}

void IOAPIC::forbid(unsigned char slot)
{
    unsigned int offset = IOAPIC_REDTBL_REG+slot*2;
    *IOREGSEL_REG = offset;
    *IOWIN_REG |= 0x10000;
}

bool IOAPIC::status(unsigned char slot)
{
    unsigned int offset = IOAPIC_REDTBL_REG+slot*2;
    *IOREGSEL_REG = offset;
    return (*IOWIN_REG&(1<<16)) ? true : false;
}
