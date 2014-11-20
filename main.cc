
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält mit den main() und main_ap() Funktionen den Startpunkt für
 *  das System
 */

/* INCLUDES */

#define DEBUG 1

#include "machine/apicsystem.h"
#include "machine/cgascr.h"
#include "machine/keyctrl.h"
#include "machine/ioapic.h"
#include "machine/cpu.h"
#include "machine/spinlock.h"

#include "object/debug.h"
#include "object/o_stream.h"

#include "device/cgastr.h"
#include "device/panic.h"
#include "device/keyboard.h"

#include "user/app1/appl.h"

#define MAIN_WIDTH 79
#define MAIN_HEIGHT 12

extern APICSystem system;

IOAPIC ioapic;
Plugbox plugbox;
Panic panic;
Keyboard keyboard;

Spinlock global;

Application app;

long j = 0;
int posX = 0;

CGA_Stream kout(0, MAIN_WIDTH, 0, MAIN_HEIGHT, false);
CGA_Stream dout_CPU0(0, 19, 13, 24, false);
CGA_Stream dout_CPU1(20, 39, 13, 24, false);
CGA_Stream dout_CPU2(40, 59, 13, 24, false);
CGA_Stream dout_CPU3(60, 79, 13, 24, false);

static const unsigned long CPU_STACK_SIZE = 4096;
// Stack fuer max. 7 APs
static unsigned char cpu_stack[(CPU_MAX - 1) * CPU_STACK_SIZE];

void *multiboot_addr = 0;

unsigned char getch()
{
    Key k;

    return k.ascii();
}

bool strcmp(char *s1, char *s2, int len)
{
    int count = 0;
    for(int i=0; i<len; ++i)
    {
        if(s1[i] == s2[i])
        {
            ++count;
        }
    }

    if(count == len)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*! \brief Einsprungpunkt ins System
 */
extern "C" int main()
{
    APICSystem::SystemType type = system.getSystemType();
    unsigned int numCPUs = system.getNumberOfCPUs();

    //TODO: INIT STUFF
    ioapic.init();
    keyboard.plugin();

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

    CPU::enable_int();

    app.action();
    
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
    CPU::enable_int();
    
    app.action();
   
    return 0;
}
