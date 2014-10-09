
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse zum Zugriff auf den CGA_Screen
 */

#ifndef __screen_include__
#define __screen_include__

/*! \brief Abstraktion des CGA-Textmodus.
 *
 *  Mit Hilfe dieser Klasse kann man auf den Bildschirm des PCs zugreifen.
 *  Der Zugriff erfolgt direkt auf der Hardwareebene, d.h. über den
 *  Bildschirmspeicher bzw. die I/O-Ports der Grafikkarte.
 *
 *  Die Implementierung soll es dabei ermöglichen die Ausgaben des CGA_Screens
 *  nur auf einem Teil des kompletten CGA-Bildschrims darzustellen. Dadurch
 *  ist es möglich die Ausgaben des Programms und etwaige Debugausgaben auf
 *  dem Bildschrim zu trennen, ohne synchronisieren zu müssen.
 */
class CGA_Screen
{
private:
    CGA_Screen(const CGA_Screen &copy); // Verhindere Kopieren
     
};

#endif

