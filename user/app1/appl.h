
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Application
 */
#include "thread/thread.h"

#ifndef APPLICATION_INCLUDE
#define APPLICATION_INCLUDE
/*! \brief Die Klasse Application definiert eine Anwendung für OO-Stubs.
 */
class Application : public Thread {

private:
    Application(const Application &copy); // Verhindere Kopieren
    int id;
    char tos[4*1024];
     
public:
    Application(int i=0);
    ~Application();

    /*! \brief Enthält den Code der Anwendung
     */
    void action ();
    void setID(int i);
};

#endif
