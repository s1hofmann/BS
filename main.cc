
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält mit den main() und main_ap() Funktionen den Startpunkt für
 *  das System
 */

/* INCLUDES */

#include "machine/apicsystem.h"
#include "machine/cgascr.h"
#include "machine/keyctrl.h"
#include "object/o_stream.h"
#include "device/cgastr.h"

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

    Keyboard_Controller kc;
    Key k;

    CGA_Stream kout(10, 59, 0, 24, true);

    unsigned char attribute = CGA_Screen::attribute(CGA_Screen::BLACK, CGA_Screen::GREEN, true);

    kout.setcolor(attribute);

    kout.setpos(0, 0);
    k = kc.key_hit();
    kout << k.ascii();

    //bool exit = false;

    kout << "Test        <stream result> -> <expected>" << endl;
    kout.setpos(0, 1);
    kout << "zero:       " << 0 << " -> 0" << endl;
    kout.setpos(0, 2);
    kout << "ten:        " << (10) << " -> 10" << endl;
    kout.setpos(0, 3);
    kout << "uint max:   " << ~((unsigned int)0) << " -> 4294967295" << endl;
    kout.setpos(0, 4);
    kout << "int max:    " << ~(1<<31) << " -> 2147483647" << endl;
    kout.setpos(0, 5);
    kout << "int min:    " << (1<<31) << " -> -2147483648" << endl;
    kout.setpos(0, 6);
    kout << "some int:   " << (-123456789) << " -> -123456789" << endl;
    kout.setpos(0, 7);
    kout << "some int:   " << (123456789) << " -> 123456789" << endl;
    kout.setpos(0, 8);
    kout << "binary:     " << bin << 42 << dec << " -> 0b101010" << endl;
    kout.setpos(0, 9);
    kout << "octal:      " << oct << 42 << dec << " -> 052" << endl;
    kout.setpos(0, 10);
    kout << "hex:        " << hex << 42 << dec << " -> 0x2a" << endl;
    kout.setpos(0, 11);
    kout << "pointer:    " << ((void*)(3735928559L)) << " -> 0xdeadbeef" << endl;
    kout.setpos(0, 12);
    kout << "smiley:     " << ((char)1) << endl;
    kout.setpos(0, 13);
    kout << bin << 4 << endl;
    kout.setpos(0, 14);
    kout << bin << -4 << endl;
    kout.setpos(5, 15);
    return 0;
}

