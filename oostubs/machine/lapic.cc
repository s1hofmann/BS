
// vim: set et ts=4 sw=4:

#include "machine/lapic.h"
#include "machine/io_port.h"
 
// global object definition
LAPIC lapic;

uint32_t LAPIC::LAPIC_BASE = 0xfee00000;

void LAPIC::write(uint16_t reg, LAPICRegister_t val)
{
    *((volatile uint32_t *) (LAPIC_BASE + reg)) = val.value;
}

LAPICRegister_t LAPIC::read(uint16_t reg)
{
    LAPICRegister_t value;
    value.value = *((volatile uint32_t *) (LAPIC_BASE + reg));
    return value;
}

uint8_t LAPIC::getLAPICID()
{
    LAPICRegister_t value = read(lapicid_reg);
    if (isPentium4()) {
        // Pentium 4 or Xeon
        return value.lapicid_p4.apic_id;
    } else {
        // Pentium
        return value.lapicid_p.apic_id;
    }
}

uint8_t LAPIC::getVersion()
{
    LAPICRegister_t value = read(lapicver_reg);
    return value.lapicver.version;
}

void LAPIC::init()
{
    LAPICRegister_t value;

    // reset logical destination ID
    // can be set using setLogicalLAPICID()
    value = read(ldr_reg);
    value.ldr.lapic_id = 0;
    write(ldr_reg, value);

    // set task priority to 0 -> accept all interrupts
    value = read(tpr_reg);
    value.tpr.task_prio = 0;
    value.tpr.task_prio_sub = 0;
    write(tpr_reg, value);

    // set flat delivery mode
    value = read(dfr_reg);
    value.dfr.model = DESTINATION_MODEL_FLAT;
    write(dfr_reg, value);

    // enable APIC and disable focus processor
    value = read(spiv_reg);
    value.svr.apic_enable = LAPIC_ENABLED;
    value.svr.focus_processor_checking = FOCUS_CPU_DISABLED;
    write(spiv_reg, value);
}

void LAPIC::ackIRQ()
{
    // dummy read
    read(spiv_reg);

    // signal end of interrupt
    LAPICRegister_t eoi;
    eoi.value = 0;
    write(eoi_reg, eoi);
}

void LAPIC::setLogicalLAPICID(uint8_t id)
{
    LAPICRegister_t value = read(ldr_reg);
    value.ldr.lapic_id = id;
    write(ldr_reg, value);
}

void LAPIC::sendIPI(uint8_t destination, struct ICR_L data)
{
    // set destination in ICR_H register
    LAPICRegister_t value = read(icrh_reg);
    value.icr_h.destination_field = destination;
    write(icrh_reg, value);

    // write data to ICR register
    // write triggers the IPI send
    value.icr_l = data;
    write(icrl_reg, value);
}

bool LAPIC::isIPIDelivered()
{
    LAPICRegister_t value = read(icrl_reg);
    return value.icr_l.delivery_status == DELIVERY_STATUS_IDLE;
}

bool LAPIC::isExternalAPIC()
{
    // System Programming Guide 3A, p. 9-6
    uint8_t version = getVersion();
    if ((version & 0xf0) == 0x00) {
        return true;
    } else {
        return false;
    }
}

bool LAPIC::isLocalAPIC()
{
    // System Programming Guide 3A, p. 9-6
    uint8_t version = getVersion();
    if ((version & 0xf0) == 0x10) {
        return true;
    } else {
        return false;
    }
}

bool LAPIC::isPentium4()
{
    // System Programming Guide 3A, p. 9-6
    uint8_t version = getVersion();
    if (version == 0x14) {
        return true;
    } else {
        return false;
    }
}

