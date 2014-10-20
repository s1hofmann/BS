
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse CGA_Stream
 */

#ifndef __cgastr_include__
#define __cgastr_include__

#include "object/o_stream.h"
#include "machine/cgascr.h"

/*! \brief Darstellung verschiedener Datentypen auf dem Bildschrim
 *
 *  Die Klasse CGA_Stream ermöglicht die Ausgabe verschiedener Datentypen als
 *  Zeichenketten auf dem CGA Bildschirm eines PCs. Dazu braucht CGA_Stream nur
 *  von den Klassen O_Stream und CGA_Screen abgeleitet und endlich die Methode
 *  flush() implementiert werden. Für weitergehende Formatierung oder spezielle
 *  Effekte stehen die Methoden der Klasse CGA_Screen zur Verfügung.
 */

class CGA_Stream : public CGA_Screen, public O_Stream
{
    public:
        CGA_Stream(int from_col, int to_col, int from_row, int to_row, bool use_cursor=false);
        virtual ~CGA_Stream();

        virtual void flush();

    private:
        CGA_Stream(CGA_Stream &copy); // Verhindere Kopieren
     
};

#endif
