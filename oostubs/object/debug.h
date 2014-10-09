
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

extern CGA_Stream dout;

#ifdef DEBUG
#define DBG dout << "DBG: "
#else
#define DBG if (false) dout
#endif

#endif // __DEBUG_H__
