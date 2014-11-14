// vim: set et ts=4 sw=4:


volatile uint32_t * IOAPIC::IOREGSEL_REG = (volatile uint32_t*) 0xfec00000;
volatile uint32_t * IOAPIC::IOWIN_REG = (volatile uint32_t*) 0xfec00010;

//IOAPIC::IOAPIC();
void IOAPIC::init(){

}
void IOAPIC::config(unsigned char slot, Plugbox::Vector vector);
void IOAPIC::allow (unsigned char slot);
void IOAPIC::forbid (unsigned char slot);
bool IOAPIC::status (unsigned char slot);

