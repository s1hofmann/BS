// vim: set et ts=4 sw=4:
#include "types.h"
#include "machine/plugbox.h"
#include "machine/ioapic_registers.h"

class IOAPIC {

public:

    /* public methods */
    //Konstruktor Tut nichts. Initialisierung erfolgt mit init() 
//    IOAPIC();
    // Initialisierung der IOAPICs. 
    void init();
    // Zuordnung eines Vektors in der Interruptvektortabelle zu einem externen Interrupt. 
    void config(unsigned char slot, Plugbox::Vector vector);
    // Sorgt dafür, dass Unterbrechungen des zugeordneten Gerätes an die CPU(s) weitergereicht werden.
    // Um eine Unterbrechungsbehandlung zu ermöglichen, muss zusätzlich CPU::enable_int() aufgerufen werden. 
    void allow (unsigned char slot);
    //  Ermöglicht einzelne Interrupts selektiv zu sperren. 
    void forbid (unsigned char slot);
    // Ermöglicht eine Abfrage des Maskierungsstatus für einzelne Interrupts. 
    bool status (unsigned char slot);

    /* static public attributes */

    // Memory-Mapped Register des IO-APIC im Adressraum der CPU. 
    static volatile uint32_t* IOREGSEL_REG;
    static volatile uint32_t* IOWIN_REG;

private:

};
// id on apic-bus
union io_apicid {
    uint32_t val;
    struct ioapic_id {
        uint32_t reserved1: 24,
                 id: 4,
                 reserved2: 4;
    } __attribute__((packed));
};
// indices of internal registers of ioapic
enum ioapic_regs_idx {
    ioapic_id,
    ioapic_cver,
    ioapic_arb,
    /* some regs left out */
    rt_start = 0x10,
};
