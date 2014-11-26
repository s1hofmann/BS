
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Diese Datei enthält die Klasse Secure.
 */

#ifndef __Secure_include__
#define __Secure_include__

/*! \brief Die Klasse Secure dient dem bequemen Schutz kritischer Abschnitte.
 *
 *  Dabei wird die Tatsache ausgenutzt, dass der C++ Compiler für jedes Objekt
 *  automatisch Konstruktor- und Destruktoraufrufe in den Code einbaut und dass
 *  ein Objekt seine Gültigkeit verliert, sobald der Bereich (Scope), in dem es
 *  deklariert wurde, verlassen wird.
 *
 *  Wenn im Konstruktor von Secure also ein kritischer Abschnitt betreten und
 *  im Destruktor wieder verlassen wird, kann die Markierung kritischer
 *  Codebereiche ganz einfach folgendermaßen erfolgen:
 *
 *  \verbatim
    // unkritisch
    ...
    { Secure section;
       // hier kommen die kritischen Anweisungen
       ...
    }
    // Ende des kritischen Abschnitts
    \endverbatim
 */
class Secure
{
private:
    Secure(const Secure &copy); // Verhindere Kopieren
     
};

#endif
