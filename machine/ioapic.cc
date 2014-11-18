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
    IOAPICRegister l;
    IOAPICRegister h;
    IOAPICRegister i;

    //Every device which connects to the APIC bus need an ID
    i.IOAPICID.ID = system.getIOAPICID();

    *IOREGSEL_REG = IOAPIC_ID_REG;
    *IOWIN_REG = i.value;

    //Vectors 0 to 31 are use for x86 exception handling, so I'll use just the first usable one
    //Plugbox initially hooks up all vectors to panic anyways, so basically it can be a random 
    //number between 32 and 255
    l.IOREDTBL_L.vector = 32;
    l.IOREDTBL_L.delivery_mode = DELIVERY_MODE_LOWESTPRI;
    l.IOREDTBL_L.destination_mode = DESTINATION_MODE_LOGICAL;
    l.IOREDTBL_L.mask = MASK_DISABLED;
    l.IOREDTBL_L.polarity = POLARITY_HIGH;
    l.IOREDTBL_L.trigger_mode = TRIGGER_MODE_EDGE;

    h.IOREDTBL_H.logical_destination = 0xff;
    h.IOREDTBL_H.reserved = 0;

    //Randomly choosing a slot is NOT the best way to init the IOAPIC
    //After reading through the docs I changed it to the appropriate keyboard slot
    //returned by getIOAPICSlot()
    
    //Calculate offset, 64 bit (2 registers) per entry
    for(int i=0; i<24; ++i)
    {
        unsigned int offset = IOAPIC_REDTBL_REG+i*2;
        //Write low part
        *IOREGSEL_REG = offset;
        *IOWIN_REG = l.value;
        //Move 32 bits (one register) and write high part
        *IOREGSEL_REG += 1;
        *IOWIN_REG = h.value;
    }
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
