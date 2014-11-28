
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Diese Datei enthält die Klasse Guard.
 */

#ifndef __Guard_include__
#define __Guard_include__

#include "object/queue.h"
#include "locker.h"

/*! \brief Synchronisation des BS-Kerns mit Unterbrechungen.
 *
 *  Die Klasse Guard dient der Synchronisation zwischen "normalen"
 *  Kernaktivitäten (zur Zeit Ausgaben, später Systemaufrufe) und
 *  Unterbrechungsbehandlungsroutinen. Dazu besitzt Guard eine Warteschlange
 *  (Queue-Objekt) je CPU, in die Gate-Objekte eingereiht werden können. Das ist
 *  immer dann erforderlich, wenn zum Zeitpunkt des Auftretens einer
 *  Unterbrechung der kritische Abschnitt gerade besetzt ist, die epilogue()
 *  Methode also nicht sofort bearbeitet werden darf. Die angesammelten
 *  Epiloge werden behandelt, sobald der kritische Abschnitt wieder
 *  freigegeben wird.
 *
 *  \b Hinweise:
 *  <ul>
 *      <li>Die Epilogqueue stellt eine zentrale Datenstruktur dar, deren
 *      Konsistenz geeignet gesichert werden muß. Die von uns durch die Klasse
 *      Queue bereitgestellte Implementierung ist nicht
 *      unterbrechungstransparent!  Entweder ihr implementiert also selbst eine
 *      unterbrechungstransparente Queue, oder ihr synchronisiert die
 *      bereitgestellte Queue entsprechend hart.</li>
 *
 *      <li>In MPStuBS benotigt man für jeden Prozessor eine eigene
 *      Epilogqueue, über die jeder Prozessor "seine" Epiloge serialisiert.
 *      Allerdings könnten dann Epiloge auf unterschiedlichen Kernen parallel
 *      laufen, da der kritische Abschnitt prozessorweise getrennt
 *      verwaltet wird. Dies muss verhindert werden, indem man beim Betreten
 *      des kritischen Abschnittes diesen mit einem Spinlock vor paralleler
 *      Abarbeitung auf anderen Prozessoren schützt</li>
 *
 *	    <li>Da Gate Objekte nur einen einzigen Verkettungszeiger besitzen,
 *	    dürfen sie zu einem Zeitpunkt nur ein einziges Mal in der Epilogliste
 *	    aufgeführt sein. Wenn also zwei gleichartige Interrupts so schnell
 *	    aufeinanderfolgen, dass der zugehörige Epilog noch gar nicht behandelt
 *	    wurde, darf nicht versucht werden, dasselbe Gate Objekt zweimal in die
 *	    Epilogliste einzutragen. Die Klasse Gate  bietet Methoden, dies zu
 *	    vermerken bzw. zu prüfen.</li>
 *
 *	    <li>Ein Betriebssystem sollte Unterbrechungen immer nur so kurz wie
 *	    möglich sperren. Daher sieht das Pro-/Epilog-Modell vor, dass Epiloge
 *	    durch Prologe unterbrochen werden können. Für OOStuBS bedeutet das,
 *	    dass bereits vor der Ausführung des Epilogs einer
 *	    Unterbrechungsbehandlung Interrupts wieder zugelassen werden
 *	    sollen.</li>
 *    </ul>
 */
class Guard : public Locker
{
private:
    Guard (const Guard &copy); // Verhindere Kopieren

    //One epilogue queue for each CPU
    //A hardcoded number of queues is not that cool.
    //APICSystem maybe provides a method to get the total amount of CPUs?
    Queue epilogues[4]
public:
    /*! \brief Konstruktor
     */
    Guard() : Locker() {};
    ~Guard() {};
    
    void enter();
    void leave();
    void relay(Gate *item);
     
};

#endif
