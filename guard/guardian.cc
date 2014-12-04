
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#include "types.h"
#include "machine/lapic.h"
#include "object/debug.h"
#include "machine/plugbox.h"
#include "guard/guard.h"
#include "machine/cpu.h"

extern Guard globalGuard;
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


    Gate* g = plugbox.report(vector);
    if(g->prologue()){
        /* relay or enqueue */

    	lapic.ackIRQ(); // tell local apic that we got the irq
        globalGuard.relay(g);
    } else {

    	lapic.ackIRQ(); // tell local apic that we got the irq
	}
}

