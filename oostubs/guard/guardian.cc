
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#include "types.h"
 
extern "C" void guardian(uint32_t vector);

/*! \brief Low-Level Interrupt-Behandlung.
 *
 *  Zentrale Unterbrechungsbehandlungsroutine des Systems.
 *  \param vector gibt die Nummer des aufgetretenen Interrupts an.
 */
void guardian(uint32_t vector)
{
     
}

