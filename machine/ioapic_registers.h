// vim: set et ts=4 sw=4
#include "types.h"
#ifndef IOAPIC_REGISTERS_H
#define IOAPIC_REGISTERS_H

#define IOAPIC_ID_REG 0x00
#define IOAPIC_VER_REG 0x01
#define IOAPIC_ARB_REG 0x02
#define IOAPIC_REDTBL_REG 0x10

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

struct IOREDTBL_L {
    uint32_t vector:8,
            delivery_mode:3,
            destination_mode:1,
            delivery_status:1,
            polarity:1,
            remote_irr:1,
            trigger_mode:1,
            mask:1,
            reserved:15;
} __attribute__((packed));

union IORT_L {
    uint32_t entry_low;
    struct IOREDTBL_L attrs;
};

struct IOREDTBL_H {
    uint32_t reserved:24,
            logical_destination:8;
} __attribute__((packed));

union IORT_H {
    uint32_t entry_high;
    struct IOREDTBL_H attrs;
};


struct IOREDTBL_ENTRY {
    struct IOREDTBL_L;
    struct IOREDTBL_H;
};


#endif /* include guard for ioapic_registers.h */
