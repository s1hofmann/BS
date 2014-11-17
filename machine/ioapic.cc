// vim: set et ts=4 sw=4:
#include "ioapic.h"
#include "apicsystem.h"
#include "ioapic_registers.h"
#define DEBUG 1
#include "object/debug.h"
#include "cpu.h"

extern APICSystem system;

// global object definition
IOAPIC ioapic;

volatile uint32_t* IOAPIC::IOREGSEL_REG = (volatile uint32_t*) 0xfec00000;
volatile uint32_t* IOAPIC::IOWIN_REG = (volatile uint32_t*) 0xfec00010;


void print_rt_entry(union IORT_L entry){
    DBG << "irq_msk: " << entry.attrs.mask << endl;
    DBG << "trig: " << entry.attrs.trigger_mode << endl;
    DBG << "rem_irr: " << entry.attrs.remote_irr << endl;
    DBG << "polar: " << entry.attrs.polarity << endl;
    DBG << "delstat: " << entry.attrs.delivery_status << endl;
    DBG << "destmod: " << entry.attrs.destination_mode << endl;
    DBG << "delmod: " << entry.attrs.delivery_mode << endl;
}

void IOAPIC::init(){
    DBG << "ioapic init" << endl;
    /* set IOAPIC ID */
    *IOAPIC::IOREGSEL_REG = ioapic_id;
    *IOAPIC::IOWIN_REG = system.getIOAPICID() << 4;

    //uint8_t rt_offset = system.getIOAPICSlot(system.keyboard);

    for(int i = 0; i < 24; i++){
        /* set low part */
        *IOAPIC::IOREGSEL_REG = rt_start + (2 * i);
        union IORT_L rt_low;
        rt_low.entry_low = *IOAPIC::IOWIN_REG;

        rt_low.attrs.mask = 1; // inactive
        rt_low.attrs.trigger_mode = 0; // Flankenstreuerung
        rt_low.attrs.polarity = 0; // Active High
        rt_low.attrs.destination_mode = 1; // Logical Mode
        rt_low.attrs.delivery_mode = 1; // Lowest Priority
        rt_low.attrs.vector = 0xff; // panic handler
        *IOAPIC::IOWIN_REG = rt_low.entry_low;

        /* set high part */
        *IOAPIC::IOREGSEL_REG = rt_start + (2 * i) + 1;
        union IORT_H rt_high;
        rt_high.entry_high = *IOAPIC::IOWIN_REG;

        rt_high.attrs.logical_destination = 0xff; // Potentiell jede CPU ist Empfaenger
        *IOAPIC::IOWIN_REG = rt_high.entry_high;
    }

}

void IOAPIC::config(unsigned char slot, Plugbox::Vector vector){
    DBG << "ioapic config" << endl;
    DBG << "slot: " << (uint32_t) slot << endl;
    DBG << "vector: " << bin << (uint32_t) vector << endl;
    *IOAPIC::IOREGSEL_REG = rt_start + (2 * slot);
    *IOAPIC::IOWIN_REG = (*IOAPIC::IOWIN_REG & 0xffffff00) | (uint32_t) vector;
    DBG << bin << *IOAPIC::IOWIN_REG << dec << endl;
}

/* enables irqs for device connected to slot
 */
void IOAPIC::allow (unsigned char slot){
    DBG << "ioapic allow" << endl;
    DBG << "slot: " << (uint32_t) slot << endl;
    *IOAPIC::IOREGSEL_REG = rt_start + (2 * slot);
    *IOAPIC::IOWIN_REG = *IOAPIC::IOWIN_REG & 0xfffeffff;
    CPU::enable_int(); // where to place this?
}

/* disables irqs for device connected to slot
 */
void IOAPIC::forbid (unsigned char slot){
    *IOAPIC::IOREGSEL_REG = rt_start + (2 * slot);
    *IOAPIC::IOWIN_REG = *IOAPIC::IOWIN_REG | 0x10000;
}

/* returns true if irqs for device connected to slot
 * are active, false otherwise
 */
bool IOAPIC::status (unsigned char slot){
    *IOAPIC::IOREGSEL_REG = rt_start + (2 * slot);
    return !(*IOAPIC::IOWIN_REG & 0x10000);
}

