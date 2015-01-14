// vim: set et ts=4 sw=4:

#include "thread/assassin.h"
#include "machine/plugbox.h"
#include "thread/scheduler.h"
#include "machine/ioapic.h"
#include "syscall/guarded_scheduler.h"

extern Scheduler scheduler;
extern Plugbox plugbox;
extern IOAPIC ioapic;

void Assassin::hire()
{
    plugbox.assign(Plugbox::assassin, this);
    ioapic.allow(Plugbox::assassin);
}

void Assassin::epilogue()
{
    //Wenn aktueller Thread das dying flag gesetzt hat, wird er beendet.
    if(scheduler.active() and scheduler.active()->dying())
    {
        //Kein resume(), sondern exit(), da der Thread nicht wieder in die readyList eingetragen werden soll.
        scheduler.exit();
    }
}
