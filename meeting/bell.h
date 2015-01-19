// vim: set et ts=4 sw=4:

#ifndef __Bell_include__
#define __Bell_include__

/*! \file
 *  \brief Enthält die Klasse Bell.
 */

/*! \brief Synchronisationsobjekt zum Schlafenlegen für eine bestimmte
 *  Zeitspanne
 *
 *  Ein "Wecker" ist ein Synchronisationsobjekt, mit dem ein oder mehrere
 *  Threads sich für eine bestimmte Zeit schlafen legen können.
 */
class Bell
{
private:
    Bell(const Bell &copy); // Verhindere Kopieren
};

#endif
