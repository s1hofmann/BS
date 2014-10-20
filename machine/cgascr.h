
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse zum Zugriff auf den CGA_Screen
 */

#ifndef __screen_include__
#define __screen_include__

#include "io_port.h"

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
    public:
        enum color
        {
            BLACK,
            BLUE,
            GREEN,
            CYAN,
            RED,
            MAGENTA,
            BROWN,
            LIGHT_GREY,
            DARK_GREY,
            LIGHT_BLUE,
            LIGHT_GREEN,
            LIGHT_CYAN,
            LIGHT_RED,
            LIGHT_MAGENTA,
            YELLOW,
            WHITE
        };

        enum
        {
            STD_ATTR = BLACK << 4 | LIGHT_GREY
        };

        enum
        {
            ROWS = 25,
            COLUMNS = 80
        };

        CGA_Screen(int from_col, int to_col, int from_row, int to_row, bool use_cursor=false);

        void setpos(int x, int y);

        void getpos(int &x, int &y);
        
        unsigned char attr; //Screen attribute

        /**
         * Draws a frame around the screen area.
         */
        void drawFrame();

        /**
         * Calculates the video memory offset with respect to x and y coordinates
         */
        int getoffset(int x, int y);

        /**
         * Clears the whole screen (testing purposes only)
         */
        void cls(char x);

        /**
         * Prints a single character at the given coordinates with respect to the 'windows' origin
         */
        void show(int x, int y, char character, unsigned char attrib=STD_ATTR);

        /**
         * Prints a string at the current position set via setpos(x,y)
         */
        void print(char *string, int length, unsigned char attrib=STD_ATTR);

        static unsigned char attribute(CGA_Screen::color bg, CGA_Screen::color fg, bool blink);

        static char *const CGA_START;

    private:
        CGA_Screen(const CGA_Screen &copy); // Verhindere Kopieren

        int scr_fx;         //Left boundary of screen object
        int scr_fy;         //Upper boundary of screen object
        int scr_tx;         //Right boundary of screen object
        int scr_ty;         //Lower boundary of screen object

        bool scr_cursor;    //Use hardware cursor?

        int pos_x;          //Cursor position X
        int pos_y;          //Cursor position Y

};

#endif

