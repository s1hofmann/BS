
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse CPU
 */

#ifndef CPU_INCLUDE
#define CPU_INCLUDE

#include "types.h"

/*! \brief Implementierung einer Abstraktion fuer den Prozessor.
 *
 *  Derzeit wird angeboten: Interrupts zuzulassen, zu verbieten, den
 *  Prozessor in den Haltmodus zu schicken oder ganz anzuhalten.
 */
class CPU
{
private:
    CPU(const CPU &copy); // Verhindere Kopieren
    /// \brief Konstruktor
    CPU() {}
public:
     
    /*! \brief Erlauben von (Hardware-)Interrupts
     *
     *  Lässt die Unterbrechungsbehandlung zu, indem die Assembleranweisung
     *  \b sti ausgeführt wird.
     */
    static void enable_int() {
        asm volatile("sti\n\t nop\n\t":::);
    }
    /*! \brief Interrupts werden ignoriert/verboten
     *
     *  Verhindert eine Reaktion auf Unterbrechungen, indem die
     *  Assembleranweisung \b cli ausgeführt wird. Der Rückgabewert gibt an ob
     *  Unterbrechungen schon gesperrt waren.
     *  \return true wenn die Unterbrechungen zum Zeitpunkt des aufrufs frei waren, false wenn sie schon gesperrt waren.
     */
    static bool disable_int() {
        uint32_t out;
        asm volatile (
            "pushf\n\t"
            "pop %0\n\t"
            "cli\n\t"
            :"=r"(out)
            :
            :
        );

        bool enabled = (out & 0x200) != 0;
        return enabled;
    }
    /*! \brief Unterbrechungszustand wiederherstellen
     *
     *  Zusammen mit dem Rückgabewert von CPU::disable_int() kann
     *  diese Funktion verwendet werden, um das Sperren und freigeben
     *  von Unterbrechungen zu schachteln.
     *  \param val Gibt an ob Unterbrechungen wieder freigegeben werden sollen oder nicht.
     */
    static void restore_int(bool val) {
        if(val) {
            enable_int();
        }
    }
    /*! \brief Prozessor bis zum nÃ¤chsten Interrupt anhalten
     *
     *  Versetzt den Prozessor in den Haltezustand, aus dem er nur durch einen
     *  Interrupt wieder erwacht. Intern werden dazu die Interrupts mit \b sti
     *  freigegeben und der Prozessor mit \b hlt angehalten. Intel garantiert,
     *  dass die Befehlsfolge \b sti \b hlt atomar ausgeführt wird.
     */
    static void idle() {
        asm volatile("sti\n\t hlt\n\t":::);
    }
    /*! \brief Prozessor dauerhaft anhalten
     *
     *  Hält den Prozessor an. Intern werden dazu die Interrupts mit \b cli
     *  gesperrt und anschließend der Prozessor mit \b hlt angehalten. Da der
     *  Haltezustand nur durch einen Interrupt verlassen werden könnte, ist
     *  somit garantiert, dass die CPU bis zum nächsten Kaltstart "steht".
     *  Das Programm kehrt aus die() nie zurück. In einer
     *  Multiprozessorumgebung hat die Ausführung der Funktion nur
     *  Auswirkungen auf die CPU, die ihn ausführt. Die anderen CPUs laufen
     *  weiter.
     */
    static void die() {
        asm volatile("cli\n\t hlt\n\t":::);
    }

    /*! \brief Prozessor anhalten
     *
     *  Hält den Prozessor an. Intern wird dazu die \b hlt Instruktion ausgeführt.
     *  In diesem Zustand verbraucht der Prozessor sehr wenig Strom.
     *  Nur ein Interrupt kann den Prozessor aufwecken.
     */
    static void halt() {
        asm volatile("hlt\n\t":::);
    }
};

#endif
