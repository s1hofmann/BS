
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Application
 */

#ifndef APPLICATION_INCLUDE
#define APPLICATION_INCLUDE

#include "thread/thread.h"
#include "machine/spinlock.h"

/*! \brief Die Klasse Application definiert eine Anwendung für OO-Stubs.
 */
class Application : public Thread
{
private:
    Application(const Application &copy); // Verhindere Kopieren
    int id;
    char runstack[4000];
     
public:
    Application();
    ~Application();
    double rand(void);

    void setID(int newId) { id = newId; }

    /*! \brief Enthält den Code der Anwendung
     */
    void action ();
};

#endif
