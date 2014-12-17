
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#define DEBUG

#include "types.h"

#include "machine/lapic.h"
#include "machine/plugbox.h"
#include "guard/guard.h"
#include "machine/cpu.h"

extern Guard guard;
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
        guard.relay(g);
    } else {

    	lapic.ackIRQ(); // tell local apic that we got the irq
	}
}
