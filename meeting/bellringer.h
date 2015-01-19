// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Bellringer.
 */

#ifndef __Bellringer_include__
#define __Bellringer_include__

/*! \brief Verwaltung und Anstoßen von zeitgesteuerten Aktivitäten.
 *
 *  Der "Glöckner" (Bellringer) wird regelmäßig aktiviert und prüft, ob
 *  irgendwelche "Glocken" (Bell-Objekte) läuten müssen. Die Glocken befinden
 *  sich in einer Liste, die der Glöckner verwaltet. Eine geschickte
 *  Implementierung vermeidet, bei jeder Aktivierung die gesamte Liste
 *  zu durchlaufen.
 */
class Bellringer
{
private:
    Bellringer(const Bellringer &copy); // Verhindere Kopieren
public:
    /*! \brief Konstruktor.
     */
    Bellringer() {}
};

#endif
