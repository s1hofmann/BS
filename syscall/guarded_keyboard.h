// vim: set et ts=4 sw=4:

#ifndef __Guarded_Keyboard_include__
#define __Guarded_Keyboard_include__

/*! \file
 *  \brief Enthält die Klasse Guarded_Keyboard
 */

/*! \brief Systemaufrufschnittstelle zur Tastatur
 */
class Guarded_Keyboard
{
private:
    Guarded_Keyboard(const Guarded_Keyboard &copy); // Verhindere Kopieren
public:
    Guarded_Keyboard() {}
};

#endif
