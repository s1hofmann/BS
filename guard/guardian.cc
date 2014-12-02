
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */

#define DEBUG

#include "types.h"

#include "machine/lapic.h"
#include "machine/plugbox.h"
#include "machine/cpu.h"

#include "object/debug.h"

#include "guard/guard.h"

extern "C" void guardian(uint32_t vector);

extern LAPIC lapic;
extern Plugbox plugbox;
extern Guard guard;

/*! \brief Low-Level Interrupt-Behandlung.
 *
 *  Zentrale Unterbrechungsbehandlungsroutine des Systems.
 *  \param vector gibt die Nummer des aufgetretenen Interrupts an.
 */
void guardian(uint32_t vector)
{
    bool epilogue_val = plugbox.report(vector)->prologue();

    lapic.ackIRQ();

    if(epilogue_val)
    {
        guard.relay(plugbox.report(vector));
        CPU::enable_int();
    }
}
