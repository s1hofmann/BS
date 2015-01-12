// vim: set et ts=4 sw=4:

/*! \file
 * \brief Enthält die Klasse LAPIC zum Zugriff auf den Local APIC
 *
 *  Local APIC referenziert in Intel System Programming Guide 3A
 */

#ifndef __LAPIC_H__
#define __LAPIC_H__

#include "types.h"
#include "machine/lapic_registers.h"
/*! \brief Abstraktion des in der CPU integrierten local APICs.
 *
 *  In modernen PCs besitzt jede CPU einen sogenannten "local APIC". Dieser
 *  vermittelt zwischen dem I/O APIC, an den die externen Interruptquellen
 *  angeschlossen sind, und der CPU. Interruptnachrichten, welche den
 *  lokalen APIC von aussen erreichen, werden an den zugeordneten Prozessorkern
 *  weitergereicht, um dort die Interruptbearbeitung anzustoßen.
 *
 *  In Multiprozessorsystem ist es darüberhinaus noch möglich mit Hilfe des
 *  lokalen APICs Nachrichten in Form von Interprozessorinterrupts an andere
 *  CPUs zu schicken bzw. zu empfangen.
 */
class LAPIC
{
private:
    void write(uint16_t reg, LAPICRegister_t value);
    LAPICRegister_t read(uint16_t reg);

    /// System Programming Guide 3A, p. 9-8 - 9-10
    enum {
        lapicid_reg			= 0x020, // Local APIC ID Register, R/W
        lapicver_reg		= 0x030, // Local APIC Version Register, RO
        tpr_reg				= 0x080, // Task Priority Register, R/W
        eoi_reg				= 0x0b0, // EOI Register, WO
        ldr_reg				= 0x0d0, // Logical Destination Register, R/W
        dfr_reg				= 0x0e0, // Destination Format Register, bits 0-27 RO, bits 28-31 R/W
        spiv_reg			= 0x0f0, // Spurious Interrupt Vector Register, bits 0-8 R/W, bits 9-1 R/W
        icrl_reg			= 0x300, // Interrupt Command Register 1, R/W
        icrh_reg			= 0x310, // Interrupt Command Register 2, R/W
        timectrl_reg		= 0x320, // LAPIC timer contol register, R/W
        timeicnt_reg	    = 0x380, // LAPIC timer initial counter register, R/W
        timeccnt_reg		= 0x390, // LAPIC timer current counter register, RO
        timediv_reg         = 0x3e0, // LAPIC timer clock divider register, RW
    };

public:

    /*! \brief Basisadresse der local APIC-Register
     */
    static uint32_t LAPIC_BASE;

    /// \brief Konstruktor
    LAPIC() {}
    /// \brief Initalisiert den local APIC der jeweiligen CPU
    void init();
    /*! \brief Signalisiert EOI(End of interrupt)
     *
     *  Teilt dem local APIC mit, dass die aktuelle Interruptbehandlung
     *  abgeschlossen ist. Diese Funktion muss gegen Ende der
     *  Unterbrechungsbehandlung aufgerufen werden und zwar bevor
     *  prozessorseitig die Unterbrechungen wieder zugelassen werden.
     */
    void ackIRQ();
    /*! \brief Liefert die ID des in der aktuellen CPU integrieren APICs
     *  \return lAPIC ID
     */
    uint8_t getLAPICID();
    /*! \brief Liefert eindeutige Identifikation der jeweiligen CPU
     *  \return lAPIC ID als Identifikation der CPU
     */
    uint8_t getCPUID() {
        return getLAPICID();
    }
    /*! \brief Liefert Versionsnummer des local APICs
     *  \return Versionsnummer
     */
    uint8_t getVersion();
    /*! \brief Setzt die local APIC ID im LDR Register
     *  \param id APIC ID
     */
    void setLogicalLAPICID(uint8_t id);
    /*! \brief Verschickt einen IPI an die adressieren CPU(s)
     *  Zum schicken von "normalen" IPIs sind folgende Einstellung im
     *  struct ICR_L notwerndig:
     *	- \b destination_shorthand: 0
     *	- \b level : 1	(Kein Init-IPI)
     *	- \b destination_mode : 1 (Logical Destination Mode)
     *	- \b delivery_mode : 0 (Fixed Delivery Mode)
     *	- \b vector : Anwendungsspezifisch
     *
     *  \param destination Maske mit Zielcpu(s)
     *  \param data Einstellungen
     */
    void sendIPI(uint8_t destination, struct ICR_L data);
    /*! \brief Kehrt mit true zurück, falls zum Zeitpunkt des Aufrufs kein IPI
     *  aktiv ist. Kehrt mit false zurück, falls der letzte gesendete IPI noch
     *  nicht vom Zielprozessor akzeptiert wurde.
     *  \return
     */
    bool isIPIDelivered();
    /// \brief Ist dieser lAPIC extern?
    bool isExternalAPIC();
    /// \brief Ist dieser lAPIC intern?
    bool isLocalAPIC();
    /// \brief Ist diese CPU ein PentiumIV?
    bool isPentium4();
    /*! \brief Ermittelt die Frequenz des LAPIC-Timers.
     *  \return Anzahl der Timerticks pro Millisekunde
     *  
     */
    uint32_t timer_ticks();
    /*! \brief Berechnet die Bitmaske für den Teiler des LAPIC-Timers.
     *  \param div Teiler, Möglichkeiten: 1, 2, 4, 8, 16, 32, 64, 128
     *  \return Bitmaske für LAPIC::setTimer() oder 0xff falls \b div
     *  ungültig ist.
     */
    uint8_t timer_div(uint8_t div);
    /*! \brief Stellt den LAPIC-Timer ein.
     *  \param counter Startwert des Zählers, der bei jedem Bus-Takt dekrementiert wird.
     *  \param divide Teiler durch den der Bustakt geteilt werden soll (siehe auch LAPIC::timer_div)
     *  \param vector Nummer des auszulösenden Interrupts.
     *  \param periodic Gibt an, ob die Unterbrechung periodisch kommen soll
     *  \param masked Unterdrückt Unterbrechungen bei Zählerablauf
     */
    void setTimer(uint32_t counter, uint8_t divide, uint8_t vector, bool periodic, bool masked=false);
};

// global object declaration
extern LAPIC lapic;

#endif
