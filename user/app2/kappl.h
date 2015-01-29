
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse KeyboardApplication
 */

#ifndef KEYBOARD_APPLICATION_INCLUDE
#define KEYBOARD_APPLICATION_INCLUDE

#include "thread/thread.h"

/*! \brief Die Klasse KeyboardApplication definiert eine Anwendung für OO-Stubs.
 */
class KeyboardApplication : public Thread
{
private:
    KeyboardApplication(const KeyboardApplication &copy); // Verhindere Kopieren
    int id;
    char runstack[4000];
     
public:
    KeyboardApplication();
    ~KeyboardApplication();
    void action ();
    void setID(int newId) { id = newId; }
    int getID() { return id; }
};

#endif
