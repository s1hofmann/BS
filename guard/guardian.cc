
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#include "types.h"
#include "machine/lapic.h"
#include "machine/plugbox.h"
#include "machine/cpu.h"
#define DEBUG
#include "object/debug.h"

extern LAPIC lapic;
extern Plugbox plugbox;

extern "C" void guardian(uint32_t vector);

/*! \brief Low-Level Interrupt-Behandlung.
 *
 *  Zentrale Unterbrechungsbehandlungsroutine des Systems.
 *  \param vector gibt die Nummer des aufgetretenen Interrupts an.
 */
void guardian(uint32_t vector)
{
    DBG << "guardian entr" << endl;
    DBG << "vector: " << vector << endl;
    Gate* gate = plugbox.report(vector);
    gate->trigger();
    lapic.ackIRQ(); // does this lapic automagically refer to actual cpu's lapic?
    DBG << "guardian lev" << endl;
    CPU::enable_int();
}

