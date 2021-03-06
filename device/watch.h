    // vim: set et ts=4 sw=4:

    /*! \file
     *  \brief Enthält die Klasse Watch
     */

#ifndef __watch_include__
#define __watch_include__

    /* INCLUDES */

#include "types.h"
#include "guard/gate.h"
#include "machine/apicsystem.h"

    /*! \brief Interruptbehandlung für Timerinterrupts.
     *
     *  Die Klasse Watch sorgt für die Behandlung der Zeitgeberunterbrechungen,
     *  indem sie eine Zeitscheibe verwaltet und gegebenenfalls einen Threadwechsel
     *  auslöst.
     */
    class Watch : public Gate
    {
    private:
        Watch (const Watch &copy); // Verhindere Kopieren

        uint32_t interval_ = 0;
        uint32_t divider_ = 1;
        uint64_t ticks_ = 0;
        int time[CPU_MAX];

        public:
            Watch();
            ~Watch();

        bool windup(uint32_t us);
        bool prologue();
        void epilogue();
        uint32_t interval();
        void activate();
    };

#endif
