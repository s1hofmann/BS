
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#include "types.h"
#include "machine/lapic.h"
#include "object/debug.h"
#include "machine/plugbox.h"

extern "C" void guardian(uint32_t vector);

extern LAPIC lapic;
extern Plugbox plugbox;

/*! \brief Low-Level Interrupt-Behandlung.
 *
 *  Zentrale Unterbrechungsbehandlungsroutine des Systems.
 *  \param vector gibt die Nummer des aufgetretenen Interrupts an.
 */
void guardian(uint32_t vector)
{
    bool epilogue_val = plugbox.report(vector)->prologue();

    if(epilogue_val)
    {
        plugbox.report(vector)->epilogue();
    }
    lapic.ackIRQ();
}

