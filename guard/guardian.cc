
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */

#define DEBUG

#include "types.h"

#include "machine/lapic.h"
#include "machine/plugbox.h"
<<<<<<< HEAD
#include "machine/cpu.h"

#include "object/debug.h"

#include "guard/guard.h"
=======
#include "guard/guard.h"
#include "machine/cpu.h"
>>>>>>> testing_marcel_a3

extern Guard globalGuard;
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
<<<<<<< HEAD
    bool epilogue_val = plugbox.report(vector)->prologue();

    lapic.ackIRQ();
=======


    Gate* g = plugbox.report(vector);
    if(g->prologue()){
        /* relay or enqueue */

    	lapic.ackIRQ(); // tell local apic that we got the irq
        globalGuard.relay(g);
    } else {

    	lapic.ackIRQ(); // tell local apic that we got the irq
	}
}
>>>>>>> testing_marcel_a3

    if(epilogue_val)
    {
        guard.relay(plugbox.report(vector));
    }
}
