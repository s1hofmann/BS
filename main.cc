
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält mit den main() und main_ap() Funktionen den Startpunkt für
 *  das System
 */

/* INCLUDES */

#include "machine/apicsystem.h"
#include "machine/cgascr.h"

extern APICSystem system;

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
    //DBG << "Is SMP system? " << (type == APICSystem::MP_APIC) << endl;
    //DBG << "Number of CPUs: " << numCPUs << endl;
    switch (type) {
        case APICSystem::MP_APIC: {
            //Startet die AP-Prozessoren
            for (unsigned int i = 1; i < numCPUs; i++) {
                void* startup_stack = (void *) &(cpu_stack[(i) * CPU_STACK_SIZE]);
                //DBG << "Booting CPU " << i << ", Stack: " << startup_stack << endl;
                
                system.bootCPU(i, startup_stack);
            }
            
        }
        case APICSystem::UP_APIC: {
            
            break;
        }
        case APICSystem::UNDETECTED: {
        }
    }
    
    return 0;
}

/*! \brief Einsprungpunkt für Applikationsprozessoren
 */
extern "C" int main_ap()
{
    //DBG << "CPU " << (int) system.getCPUID() << " in main_ap(), waiting for callout." << endl;
    system.waitForCallout();
    system.initLAPIC();
    system.callin();

    CGA_Screen test(0, 20, 0, 5);
    CGA_Screen test2(21, 40, 6, 24);

    unsigned char attr = test.attribute(test.RED, test.GREEN, true);

    test.cls('x');
    test.setpos(1, 1);
    test.print("Hello, world!", 13, attr);

    test2.cls('u');
    test2.setpos(2,2);
    test2.print("Window 2", 8);
    
    return 0;
}

