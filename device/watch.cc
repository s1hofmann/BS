// vim: set et ts=4 sw=4:

#define DEBUG

#include "device/watch.h"
#include "machine/plugbox.h"
#include "machine/ioapic.h"
#include "machine/lapic.h"
#include "object/debug.h"
#include "thread/scheduler.h"

extern Plugbox plugbox;
extern LAPIC lapic;
extern IOAPIC ioapic;
extern Scheduler scheduler;

Watch::Watch()
{
}

Watch::~Watch()
{
}

bool Watch::windup(uint32_t us)
{
    plugbox.assign(Plugbox::timer, this);
    ioapic.allow(Plugbox::timer);

    this->interval_ = us;

    //Ticks/Millisekunde des LAPIC timers
    this->ticks_ = lapic.timer_ticks();

    this->ticks_/=1000; //Ticks/Mikrosekunde
    this->ticks_*=us; //Anzahl Ticks für Intervall von us Mikrosekunden

    //Überlauf prüfen
    while(this->ticks_>static_cast<uint32_t>((2<<31)-1))
    {
        this->ticks_ = this->ticks_>>1;
        this->divider_ = this->divider_<<1;
        if(this->divider_>128)
            return false;
    }

    return true;
}

bool Watch::prologue()
{
    return true;
}

void Watch::epilogue()
{
    scheduler.schedule();
}

uint32_t Watch::interval()
{
    return this->interval_;
}

void Watch::activate()
{
    lapic.setTimer(this->ticks_, this->divider_, Plugbox::timer, true, false);
}
