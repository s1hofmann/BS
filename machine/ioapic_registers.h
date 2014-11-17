// vim: set et ts=4 sw=4
#include "types.h"
#ifndef IOAPIC_REGISTERS_H
#define IOAPIC_REGISTERS_H

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
