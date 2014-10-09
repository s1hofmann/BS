
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Application
 */

#ifndef APPLICATION_INCLUDE
#define APPLICATION_INCLUDE

/*! \brief Die Klasse Application definiert eine Anwendung für OO-Stubs.
 */
class Application
     
{
private:
    Application(const Application &copy); // Verhindere Kopieren
    int id;
     
public:
     
    /*! \brief Enthält den Code der Anwendung
     */
    void action ();
};

#endif
