
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse TxtApp
 */

#ifndef TXT_APPLICATION_INCLUDE
#define TXT_APPLICATION_INCLUDE

#include "thread/thread.h"

/*! \brief Die Klasse TxtApp definiert eine Anwendung für OO-Stubs.
 */
class TxtApp : public Thread
{
private:
    TxtApp(const TxtApp &copy); // Verhindere Kopieren
    int id;
    char runstack[4000];
     
public:
    TxtApp();
    ~TxtApp();

    void setID(int newId) { id = newId; }
    int getID() { return id; }

    /*! \brief Enthält den Code der Anwendung
     */
    void action ();
};

#endif
