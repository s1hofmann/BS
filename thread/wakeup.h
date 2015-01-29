// vim: set et ts=4 sw=4:

#define DEBUG
#include "object/debug.h"

#ifndef __WAKEUP_H__
#define __WAKEUP_H__

#include "guard/gate.h"
#include "machine/plugbox.h"

extern Plugbox plugbox;

/*! \file
 *  \brief Enthält die Klasse WakeUp
 */

/*! \brief Interruptbehandlungsobjekt, um in MPStuBS schlafende Prozessoren
 *  mit einem IPI zu wecken, falls neue Threads aktiv wurden.
 *
 *  Nur in MPStuBS benötigt.
 */
class WakeUp : public Gate
{
    public:
        void activate() { plugbox.assign(Plugbox::wakeup, this); }
        bool prologue() { DBG << "wakeup!" << endl; return false; }
};

#endif /* __WAKEUP_H__ */
