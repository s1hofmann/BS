
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält mit den main() und main_ap() Funktionen den Startpunkt für
 *  das System
 */

/* INCLUDES */

#define DEBUG

#include "machine/apicsystem.h"
#include "machine/cgascr.h"
#include "machine/keyctrl.h"
#include "machine/ioapic.h"
#include "machine/cpu.h"

#include "object/debug.h"
#include "object/o_stream.h"

#include "device/cgastr.h"
#include "device/panic.h"
#include "device/keyboard.h"

#include "guard/guard.h"

#include "thread/scheduler.h"
#include "thread/assassin.h"
#include "thread/wakeup.h"
#include "thread/idlethread.h"
#include "device/watch.h"

#include "user/app1/appl.h"
#include "user/txt/appt.h"

#include "syscall/guarded_scheduler.h"
#include "syscall/guarded_semaphore.h"

#define MAIN_WIDTH 79
#define MAIN_HEIGHT 12

extern APICSystem system;

IOAPIC ioapic;
Plugbox plugbox;
Panic panic;
Keyboard keyboard;
Scheduler scheduler;

Guard guard;
Assassin assassin;
WakeUp wakeup;
Watch watch;

Guarded_Semaphore cgaSemaphore(1);

Application app[MAIN_WIDTH];
TxtApp txt;
IdleThread idleThreads[CPU_MAX];

long j = 0;
int posX = 0;

bool watch_init;

CGA_Stream kout(0, MAIN_WIDTH, 0, MAIN_HEIGHT, false);
CGA_Stream dout_CPU0(0, 19, 13, 24, false);
CGA_Stream dout_CPU1(20, 39, 13, 24, false);
CGA_Stream dout_CPU2(40, 59, 13, 24, false);
CGA_Stream dout_CPU3(60, 79, 13, 24, false);

static const unsigned long CPU_STACK_SIZE = 4096;
// Stack fuer max. 7 APs
static unsigned char cpu_stack[(CPU_MAX - 1) * CPU_STACK_SIZE];

void *multiboot_addr = 0;

/*! \brief Einsprungpunkt ins System
 */
extern "C" int main()
{
    APICSystem::SystemType type = system.getSystemType();
    unsigned int numCPUs = system.getNumberOfCPUs();

    //INIT STUFF
    ioapic.init();
    keyboard.plugin();
    assassin.hire();
    wakeup.activate();
    //5 Sekunden Interval
    watch_init = watch.windup(5000000);

    //for(int i=0; i<MAIN_WIDTH; ++i)
    for(int i=0; i<8; ++i)
    {
        app[i].setID(i);
        scheduler.ready(&app[i]);
    }
    for(int i=0; i<CPU_MAX; ++i)
    {
        idleThreads[i].setID(1337);
        scheduler.setIdleThread(i, &idleThreads[i]);
    }

    //txt.setID(100);
    //Guarded_Scheduler::ready(&txt);

    DBG << "Is SMP system? " << (type == APICSystem::MP_APIC) << endl;
    DBG << "Number of CPUs: " << numCPUs << endl;
    switch (type) {
        case APICSystem::MP_APIC: {
            //Startet die AP-Prozessoren
            for (unsigned int i = 1; i < numCPUs; i++) {
                void* startup_stack = (void *) &(cpu_stack[(i) * CPU_STACK_SIZE]);
                DBG << "Booting CPU " << i << ", Stack: " << startup_stack << endl;
                
                system.bootCPU(i, startup_stack);
            }
        }
        case APICSystem::UP_APIC: {
            break;
        }
        case APICSystem::UNDETECTED: {
        }
    }

    if(watch_init)
        watch.activate();
    guard.enter();
    //enable_int hier, nicht wie erst oben, da sonst sofort ein Interrupt kommen kann, der einen Taskwechsel ohne vorhandenen Task ausloest
    scheduler.schedule();
    
    return 0;
}

/*! \brief Einsprungpunkt für Applikationsprozessoren
 */
extern "C" int main_ap()
{
    DBG << "CPU " << (int) system.getCPUID() << " in main_ap(), waiting for callout." << endl;
    
    system.waitForCallout();
    system.initLAPIC();
    system.callin();

    //Code in here runs on multiply CPUs
    //This caused quite a mess when dealing with keyboard input in exercise 1
    
    if(watch_init)
        watch.activate();
    guard.enter();
    scheduler.schedule();

    return 0;
}
