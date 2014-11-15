#ifndef IOAPIC_REGISTERS_H_KHFP3WEI
#define IOAPIC_REGISTERS_H_KHFP3WEI

#define IOAPICID_REG 0x00
#define IOAPICVER_REG 0x01
#define IOAPICARB_REG 0x02
#define IOAPICREDTBL_REG 0x10

#define DESTINATION_MODE_PHYSICAL   0x0
#define DESTINATION_MODE_LOGICAL   0x1
#define TRIGGER_MODE_EDGE   0x0
#define TRIGGER_MODE_LEVEL   0x1
#define POLARITY_HIGH   0x0
#define POLARITY_LOW   0x1
#define MASK_ENABLED   0x0
#define MASK_DISABLED   0x1
#define DELIVERY_MODE_FIXED   0x0
#define DELIVERY_MODE_LOWESTPRI   0x1
#define DELIVERY_MODE_SMI   0x2
#define DELIVERY_MODE_NMI   0x4
#define DELIVERY_MODE_INIT   0x5
#define DELIVERY_MODE_EXTINT   0x7

struct IOAPICID
{
    unsigned int reserved_2:24;
    unsigned int ID:4;
    unsigned int reserved_1:4;
};

struct IOREDTBL_L
{
    unsigned int vector:8;
     //Interrupt Vector, R/W.
    unsigned int delivery_mode:3;
     //Delivery Mode, R/W.
    unsigned int destination_mode:1;
     //Destination Mode, R/W.
    unsigned int delivery_status:1;
     //Delivery Status, RO.
    unsigned int polarity:1;
     //Interrupt Input Pin Polarity, R/W.
    unsigned int remote_irr:1;
     //Remote IRR (for level-triggered interrupts only), RO.
    unsigned int trigger_mode:1;
     //Trigger Mode, R/W.
    unsigned int mask:1;
     //Interrupt Mask, R/W.
    unsigned int reserved:15;
};

struct IOREDTBL_H
{
    unsigned int reserved:24;
    unsigned int logical_destination:8;
     //Menge von Zielprozessoren.
};

union IOAPICRegister
{
    struct IOAPICID IOAPICID;
    struct IOREDTBL_L IOREDTBL_L;
    struct IOREDTBL_H IOREDTBL_H;
    
    //Leck mich am Arsch hat das lange gedauert, bis ich gecheckt hab, was das hier bewirken soll...
    //Gibt Inhalt einer der obigen Structs als 32 Bit Wert aus, da Unions überlappenden Speicherbereich haben.
    unsigned int value;
};

#endif /* end of include guard: IOAPIC_REGISTERS_H_KHFP3WEI */
