// vim: set et ts=4 sw=4:
class IOAPIC {

public:

    /* public methods */
    //Konstruktor Tut nichts. Initialisierung erfolgt mit init() 
    IOAPIC();
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
    static volatile uint32_t * IOREGSEL_REG;
    static volatile uint32_t * IOWIN_REG;

private:

}
