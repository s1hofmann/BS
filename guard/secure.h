<<<<<<< HEAD
=======

>>>>>>> testing_marcel_a3
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Diese Datei enthält die Klasse Secure.
 */

<<<<<<< HEAD
#ifndef __Secure_include__
#define __Secure_include__

#define DEBUG

#include "object/debug.h"
#include "guard.h"

extern Guard guard;

/*! \briefsecure.hsse Secure dient dem bequemen Schutz kritischer Abschnitte.
=======
#include "guard/guard.h"
#ifndef __Secure_include__
#define __Secure_include__

/*! \brief Die Klasse Secure dient dem bequemen Schutz kritischer Abschnitte.
>>>>>>> testing_marcel_a3
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
<<<<<<< HEAD
=======

extern Guard globalGuard;

>>>>>>> testing_marcel_a3
class Secure
{
private:
    Secure(const Secure &copy); // Verhindere Kopieren
<<<<<<< HEAD

public:
    Secure() {guard.enter();};
    ~Secure() {guard.leave();};
=======
public:
    Secure(){ globalGuard.enter(); }
    ~Secure(){ globalGuard.leave(); }
>>>>>>> testing_marcel_a3
};

#endif
