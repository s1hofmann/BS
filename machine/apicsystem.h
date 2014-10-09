
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse APICSystem, die zum Hochfahren des
 *  Systems benötigt wird.
 */

#ifndef __APICSystem__
#define __APICSystem__

#include "machine/mp_registers.h"

/// maximum number of CPUs
#define CPU_MAX 8

struct system_configuration {
    unsigned int cpu_count;
    unsigned int bsp_id;
    int ioapic_id;
    bool disablePICs;
    //speichert zu jedem Slot am IOAPIC die Legacy ISA IRQ Nummer (APICSystem::Device
    char ioapic_irqs[24];
    //Speichert zu jeder Legacy ISA IRQ Nummer(APICSystem::Device) den Slot am
    //IOAPIC, an dem das entsprechende Gerät hängt
    char slot_map[16];
    unsigned int lapic_id[CPU_MAX];
    system_configuration() : cpu_count(0), bsp_id(0), ioapic_id(-1), disablePICs(false) {
        for (int i = 0; i < 24; ++i) {
            ioapic_irqs[i] = -1;
            if(i < 16) {
                slot_map[i] = -1;
            }
        }
    }
    void pretty_print();
};

typedef struct system_configuration SystemConfig;

/*! \brief Abstraktion für die Erkennung und das Booten eines PCs mit APIC.
 */
class APICSystem
{
public:
    ///Art des Systems
    enum SystemType {
        MP_APIC,		///< Multiprozessor System
        UP_APIC,		///< Singleprozessor System mit APIC HW
        UNDETECTED      ///< Unbekannte Systemkonfiguration
    };
    enum Device {
        timer = 0,
        keyboard = 1,
        com1 = 3,
        com2 = 4,
        floppy = 6,
        lpt1 = 7,
        rtc = 8,
        ps2mouse = 12,
        ide1 = 14,
        ide2 = 15
    };
private:
    SystemType system_type;
    unsigned int onlineCPUs; ///< running CPUs
    //unsigned int numberOfCPUs; ///< number of CPUs present in the system
    //unsigned int bspID; ///< local APIC ID of the bootstrap CPU
    //unsigned char ioapicID; ///< the ID of the detected I/O APIC, needed for programming its ID register (default: 1)
    int compatibilityMode; ///< the compatibility mode that was detected (PIC, virtual wire, symmetric I/O)
    int cpuFamily[CPU_MAX]; ///< the CPU family per CPU
    int cpuModel[CPU_MAX]; ///< the CPU model per CPU
    //unsigned int lapicID[CPU_MAX]; ///< local APIC IDs per CPU
    unsigned int lapicVersion[CPU_MAX]; ///< the version of the local APICs per CPU
    volatile char calloutVector[CPU_MAX]; ///< set by BSP to allow APs to init
    volatile char callinVector[CPU_MAX]; ///< set by APs to tell BSP they are running
    volatile unsigned char nextLogicalLAPICID; ///< the next logical ID set upon LAPIC setup (the ID, not the bitmask)
    unsigned char logicalLAPICID[CPU_MAX]; ///< logical LAPIC IDs per CPU, needed for IPI
    void *relocatedSetupAP; ///< Zeiger auf den Setupcode für die Applikation
    SystemConfig mps_conf;
    SystemConfig acpi_conf;
    SystemConfig sys_conf;
public:
    APICSystem() : system_type(UNDETECTED), onlineCPUs(1),
        nextLogicalLAPICID(0), relocatedSetupAP((void *)42) {
        // create entry point for APs
        // (setup_ap is copied to an address < 1M)
        copySetupAPtoLowMem();
    }
    /*! \brief Führt Systemerkennung aus.
     *
     *  Diese Funktion erkennt ein eventuell vorhandenes Mehrprozessorsystem.
     *  Nach erfolgter Erkennung kann der Systemtyp mit Hilfe der Methode
     *  getSystemType() abgefragt werden.
     */
    void detectSystemType();
    /*! \brief Startet einen Applikationsprozessor.
     *
     *  \param cpu_id Gib an, welcher Prozessor gestartet werden soll.
     *  Applikationsprozessoren haben die IDs 1 bis n-1
     *  \param top_of_stack Zeiger auf die oberste Adresse des Stacks, der von
     *  der zu startenden CPU verwendet werden soll.
     *  \return True, falls CPU Bootup erfolgreich; sonst false.
     */
    bool bootCPU(unsigned int cpu_id, void *top_of_stack);
    /*! \brief Liefert die CPUID der aktuellen CPU.
     *
     *  \return CPUID der aktuellen CPU.
     */
    int getCPUID();
    /*! \brief Auslösen eines Interprozessorinterrupts
     *
     *  Mit Hilfe dieser Methode kann ein Interprozessorinterrupt(IPI) an eine
     *  ganze Gruppe von Prozessoren geschickt werden. In MPStuBS ist das System
     *  derart konfiguriert, dass insgesamt 8 CPUs angesprochen werden können.
     *  Jede CPU wird wird durch ein Bit in \a logicalDestination repräsentiert.
     *  Dabei steht Bit 0 für CPU 0, Bit 1 für CPU 1 usw.. Welcher Interruptvektor
     *  dabei ausgelöst werden soll, wird durch \a vector festgelegt. Zum
     *  Senden eines IPIs soll dabei die Methode LAPIC::sendIPI() verwendet
     *  werden.
     *
     *  \param logicalDestination Menge von Prozessoren, die einen IPI erhalten
     *  sollen.
     *  \param vector Interruptvektor, der ausgelöst werden soll.
     */
    void sendCustomIPI(unsigned char logicalDestination, unsigned char vector);
    /*! \brief Initialisiert den Local APIC
     */
    void initLAPIC();
    /*! \brief Liefert die Nummer des Pins, an dem das Gerät device hängt
     */
    unsigned char getIOAPICSlot(APICSystem::Device device);
    const char *getCPUModel(unsigned int cpuID);
    /*! \brief Abfrage des Systemtyps
     *
     *  \return Gibt den Systemtyp zurück
     */
    SystemType getSystemType() {
        return system_type;
    }
    /// Gibt die Anzahl der CPUs zurück
    unsigned int getNumberOfCPUs() {
        return sys_conf.cpu_count;
    }
    /// Gibt die Anzahl der erfolgreich gebooteten CPUs zurück
    unsigned int getNumberOfOnlineCPUs() {
        return onlineCPUs;
    }
    /// Liefert die während des Bootvorgangs ausgelesene ID des IOAPICs.
    unsigned char getIOAPICID() {
        return sys_conf.ioapic_id;
    }
    /// Liefert die ID des Bootprozessors (BSP)
    unsigned char getBSPID() {
        return sys_conf.bsp_id;
    }

    void callin() {
        callinVector[getCPUID()] = 1;
    }

    void waitForCallout() {
        while (calloutVector[getCPUID()] == 0);
    }

    unsigned char getAndIncNextLogicalAPICID() {
        unsigned char res = nextLogicalLAPICID;
        logicalLAPICID[getCPUID()] = nextLogicalLAPICID;
        nextLogicalLAPICID++;
        return res;
    }

    unsigned char getLogicalLAPICID(unsigned char cpu) {
        return logicalLAPICID[cpu];
    }
private:
    void queryACPI(SystemConfig& conf); //use ACPI to detect SMP system
    bool mpbChecksumOK(unsigned char *start_address, int len);
    void readMPCT(struct mpcth *mpcth, SystemConfig& conf);
    bool searchConfig(unsigned long base, unsigned long length, SystemConfig& conf);
    void delay(unsigned int wraparounds);
    void copySetupAPtoLowMem();
    void disablePICs(); //disable PIC compatibility mode
    // finishes IRQ Map initalisation after ACPI/MPS detection
    void generateIRQMap(char ioapic_irqs[], char slot_map[]);

};

// global object declaration
extern APICSystem system;

#endif
