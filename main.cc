
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
#include "object/debug.h"

#define DEBUG

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

    Keyboard_Controller kc;
    kc.set_repeat_rate(3, 15);
    Key k;

    CGA_Stream kout(0, 79, 0, 12, true);
    CGA_Stream dout_CPU0(0, 19, 13, 24, false);
    CGA_Stream dout_CPU1(20, 39, 13, 24, false);
    CGA_Stream dout_CPU2(40, 59, 13, 24, false);
    CGA_Stream dout_CPU3(60, 79, 13, 24, false);

    unsigned char attribute = CGA_Screen::attribute(CGA_Screen::BLACK, CGA_Screen::GREEN, true);

    kout.setcolor(attribute);

    kout.setpos(0, 0);
    kout << "test" << endl;

    bool exit = false;

    while(!exit)
    {
        do
        {
            k = kc.key_hit();
        } while(!(k.valid()));
        kout << k.ascii();
        kout.flush();
    }

    kout << "Test        <stream result> -> <expected>" << endl;
    kout << "zero:       " << 0 << " -> 0" << endl;
    kout << "ten:        " << (10) << " -> 10" << endl;
    kout << "uint max:   " << ~((unsigned int)0) << " -> 4294967295" << endl;
    kout << "int max:    " << ~(1<<31) << " -> 2147483647" << endl;
    kout << "int min:    " << (1<<31) << " -> -2147483648" << endl;
    kout << "some int:   " << (-123456789) << " -> -123456789" << endl;
    kout << "some int:   " << (123456789) << " -> 123456789" << endl;
    kout << "binary:     " << bin << 42 << dec << " -> 0b101010" << endl;
    kout << "octal:      " << oct << 42 << dec << " -> 052" << endl;
    kout << "hex:        " << hex << 42 << dec << " -> 0x2a" << endl;
    kout << "pointer:    " << ((void*)(3735928559L)) << " -> 0xdeadbeef" << endl;
    kout << "smiley:     " << ((char)1) << endl;
    
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

    //Code in here runs on multiply CPUs
    //This caused quite a mess when dealing with keyboard input in exercise 1

    return 0;
}

