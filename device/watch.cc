// vim: set et ts=4 sw=4:

#define DEBUG

#include "device/watch.h"
#include "machine/plugbox.h"
//#include "machine/ioapic.h"
#include "machine/lapic.h"
#include "object/debug.h"
#include "thread/scheduler.h"
#include "utils/math.h"
#include "machine/apicsystem.h"
#include "meeting/bellringer.h"

extern Plugbox plugbox;
extern LAPIC lapic;
//extern IOAPIC ioapic;
extern Scheduler scheduler;
extern APICSystem system;
extern Bellringer ringer;

Watch::Watch()
{
    for(int i=0; i<CPU_MAX; ++i)
    {
        this->time[i]=0;
    }
}

Watch::~Watch()
{
}

bool Watch::windup(uint32_t us)
{
    //IOAPIC nicht notwendig, weil lapic eh direkt auf cpu
    //ioapic.allow(Plugbox::timer);

    this->interval_ = us;

    //Ticks/Millisekunde des LAPIC timers
    this->ticks_ = lapic.timer_ticks();

    this->ticks_*=us; //Anzahl Ticks für Intervall von us Mikrosekunden
    this->ticks_=Math::div64(this->ticks_, 1000); //Ticks/Mikrosekunde

    //Überlauf prüfen
    //Ticks auf uint32 casten und auf gleicheit pruefen
    while(this->ticks_>static_cast<uint32_t>((2<<31)-1))
    {
        this->ticks_ = this->ticks_>>1;
        this->divider_ = this->divider_<<1;
        if(this->divider_>128)
            return false;
    }

    plugbox.assign(Plugbox::timer, this);
    return true;
}

bool Watch::prologue()
{
    return true;
}

void Watch::epilogue()
{
    ++this->time[system.getCPUID()];
    //DBG << "Timer: " << dec << this->time[system.getCPUID()] << endl;
    // Nur Thread mit ID 0 den Bellringer aufrufen lassen
    if(system.getCPUID()==0)
        ringer.check();

    scheduler.resume();
}

uint32_t Watch::interval()
{
    return this->interval_;
}

void Watch::activate()
{
    lapic.setTimer(this->ticks_, this->divider_, Plugbox::timer, true, false);
}
