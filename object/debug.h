
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält Debugmacros, um Debugausgaben auf einen eigenen
 *  Screen umzuleiten.
 *
 *  Für den Uniprozessorfall reicht es ein CGA_Stream Objekt für Debugausgaben
 *  (\b dout) anzulegen. Für den Multiprozessorfall soll jedoch für jede CPU ein
 *  Objekt für Debugausgaben angelegt werden. Das Debugmacro muss dann mit Hilfe
 *  von APICSystem::getCPUID() die Ausgabe auf das entsprechende Objekt
 *  umleiten. Dazu kann der <b>? : </b>Operator verwendet werden.
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "device/cgastr.h"

extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

#include "machine/apicsystem.h"
extern APICSystem system;

#ifdef DEBUG
#define DBG ((system.getCPUID() == 0) ? dout_CPU0 : \
                ((system.getCPUID() == 1) ? dout_CPU1 : \
                ((system.getCPUID() == 2) ? dout_CPU2 : dout_CPU3))) << "DBG: "
#else
#define DBG if (false) dout_CPU0
#endif

#endif // __DEBUG_H__
