
// vim: set et ts=4 sw=4:

/* INCLUDES */

#include "machine/keyctrl.h"

#define DEBUG
#include "object/debug.h"

/* GLOBALE VARIABLEN */

/* STATIC MEMERS */

unsigned char Keyboard_Controller::normal_tab[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 225, 39, '\b',
    0, 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 129, '+', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 148, 132, '^', 0, '#',
    'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, '<', 0, 0
};

unsigned char Keyboard_Controller::shift_tab[] = {
    0, 0, '!', '"', 21, '$', '%', '&', '/', '(', ')', '=', '?', 96, 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 154, '*', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 153, 142, 248, 0, 39,
    'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,
    0, 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '>', 0, 0
};

unsigned char Keyboard_Controller::alt_tab[] = {
    0, 0, 0, 253, 0, 0, 0, 0, '{', '[', ']', '}', '\\', 0, 0,
    0, '@', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '~', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 230, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0
};

unsigned char Keyboard_Controller::asc_num_tab[] = {
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', ','
};
unsigned char Keyboard_Controller::scan_num_tab[] = {
    8, 9, 10, 53, 5, 6, 7, 27, 2, 3, 4, 11, 51
};

/* PRIVATE METHODEN */

// KEY_DECODED: Interpretiert die Make und Break-Codes der Tastatur und
//              liefert den ASCII Code, den Scancode und Informationen
//              darueber, welche zusaetzlichen Tasten wie Shift und Ctrl
//              gedrueckt wurden. Ein Rueckgabewert true bedeutet, dass
//              das Zeichen komplett ist, anderenfalls fehlen noch Make
//              oder Breakcodes.
bool Keyboard_Controller::key_decoded ()
{
    bool done = false;

    // Die Tasten, die bei der MF II Tastatur gegenueber der aelteren
    // AT Tastatur hinzugekommen sind, senden immer erst eines von zwei
    // moeglichen Prefix Bytes.
    if (code == prefix1 || code == prefix2) {
        prefix = code;
        return false;
    }

    // Das Loslassen einer Taste ist eigentlich nur bei den "Modifier" Tasten
    // SHIFT, CTRL und ALT von Interesse, bei den anderen kann der Break Code
    // ignoriert werden.
    if (code & break_bit) {
        code &= ~break_bit;     // Der Break Code einer Taste ist gleich dem
        // Make Code mit gesetzten break_bit.
        switch (code) {
            case 42:
            case 54:
                gather.SHIFT (false);
                break;
            case 56:
                if (prefix == prefix1)
                    gather.alt_right (false);
                else
                    gather.alt_left (false);
                break;
            case 29:
                if (prefix == prefix1)
                    gather.ctrl_right (false);
                else
                    gather.ctrl_left (false);
                break;
        }

        // Ein Prefix gilt immer nur fuer den unmittelbar nachfolgenden Code.
        // Also ist es jetzt abgehandelt.
        prefix = 0;

        // Mit einem Break-Code kann man nichts anfangen, also false liefern.
        return false;
    }

    // Eine Taste wurde gedrueckt. Bei den Modifier Tasten wie SHIFT, ALT,
    // NUM_LOCK etc. wird nur der interne Zustand geaendert. Durch den
    // Rueckgabewert 'false' wird angezeigt, dass die Tastatureingabe noch
    // nicht abgeschlossen ist. Bei den anderen Tasten werden ASCII
    // und Scancode eingetragen und ein 'true' fuer eine erfolgreiche
    // Tastaturabfrage zurueckgegeben, obwohl genaugenommen noch der Break-
    // code der Taste fehlt.

    switch (code) {
        case 42:
        case 54:
            gather.SHIFT (true);
            break;
        case 56:
            if (prefix == prefix1)
                gather.alt_right (true);
            else
                gather.alt_left (true);
            break;
        case 29:
            if (prefix == prefix1)
                gather.ctrl_right (true);
            else
                gather.ctrl_left (true);
            break;
        case 58:
            gather.caps_lock (!gather.caps_lock ());
            set_led (led_caps_lock, gather.caps_lock());
            break;
        case 70:
            gather.scroll_lock (!gather.scroll_lock ());
            set_led (led_scroll_lock, gather.scroll_lock());
            break;
        case 69: // Numlock oder Pause ?
            if (gather.ctrl_left ()) { // Pause Taste
                // Auf alten Tastaturen konnte die Pause-Funktion wohl nur
                // ueber Ctrl+NumLock erreicht werden. Moderne MF-II Tastaturen
                // senden daher diese Codekombination, wenn Pause gemeint ist.
                // Die Pause Taste liefert zwar normalerweise keinen ASCII
                // Code, aber Nachgucken schadet auch nicht. In jedem Fall ist
                // die Taste nun komplett.
                get_ascii_code ();
                done = true;
            } else { // NumLock
                gather.num_lock (!gather.num_lock());
                set_led (led_num_lock, gather.num_lock ());
            }
            break;

        default: // alle anderen Tasten
            // ASCII Codes aus den entsprechenden Tabellen auslesen, fertig.
            get_ascii_code ();
            done = true;
    }

    // Ein Prefix gilt immer nur fuer den unmittelbar nachfolgenden Code.
    // Also ist es jetzt abgehandelt.
    prefix = 0;

    if (done)       // Tastaturabfrage abgeschlossen
        return true;
    else
        return false;
}

// GET_ASCII_CODE: ermittelt anhand von Tabellen aus dem Scancode und
//                 den gesetzten Modifier-Bits den ASCII Code der Taste.

void Keyboard_Controller::get_ascii_code ()
{
    // Sonderfall Scancode 53: Dieser Code wird sowohl von der Minustaste
    // des normalen Tastaturbereichs, als auch von der Divisionstaste des
    // Ziffernblocks gesendet. Damit in beiden Faellen ein Code heraus-
    // kommt, der der Aufschrift entspricht, muss im Falle des Ziffern-
    // blocks eine Umsetzung auf den richtigen Code der Divisionstaste
    // erfolgen.
    if (code == 53 && prefix == prefix1) { // Divisionstaste des Ziffernblocks
        gather.ascii ('/');
        gather.scancode (Key::scan::div);
    }

    // Anhand der Modifierbits muss die richtige Tabelle ausgewaehlt
    // werden. Der Einfachheit halber hat NumLock Vorrang vor Alt,
    // Shift und CapsLock. Fuer Ctrl gibt es keine eigene Tabelle.

    else if (gather.num_lock () && !prefix && code>=71 && code<=83) {
        // Bei eingeschaltetem NumLock und der Betaetigung einer der
        // Tasten des separaten Ziffernblocks (Codes 71-83), sollen
        // nicht die Scancodes der Cursortasten, sondern ASCII und
        // Scancodes der ensprechenden Zifferntasten geliefert werden.
        // Die Tasten des Cursorblocks (prefix == prefix1) sollen
        // natuerlich weiterhin zur Cursorsteuerung genutzt werden
        // koennen. Sie senden dann uebrigens noch ein Shift, aber das
        // sollte nicht weiter stoeren.
        gather.ascii (asc_num_tab[code-71]);
        gather.scancode (scan_num_tab[code-71]);
    } else if (gather.alt_right ()) {
        gather.ascii (alt_tab[code]);
        gather.scancode (code);
    } else if (gather.SHIFT ()) {
        gather.ascii (shift_tab[code]);
        gather.scancode (code);
    } else if (gather.caps_lock ()) {
        // Die Umschaltung soll nur bei Buchstaben gelten
        if ((code>=16 && code<=26) || (code>=30 && code<=40)
                || (code>=44 && code<=50)) {
            gather.ascii (shift_tab[code]);
            gather.scancode (code);
        } else {
            gather.ascii (normal_tab[code]);
            gather.scancode (code);
        }
    } else {
        gather.ascii (normal_tab[code]);
        gather.scancode (code);
    }
}

/* OEFFENTLICHE METHODEN */

// KEYBOARD_CONTROLLER: Initialisierung der Tastatur: alle LEDs werden
//                      ausgeschaltet und die Wiederholungsrate auf
//                      maximale Geschwindigkeit eingestellt.

Keyboard_Controller::Keyboard_Controller () :
    ctrl_port (0x64), data_port (0x60)
{
    // alle LEDs ausschalten (bei vielen PCs ist NumLock nach dem Booten an)
    set_led (led_caps_lock, false);
    set_led (led_scroll_lock, false);
    set_led (led_num_lock, false);

    // maximale Geschwindigkeit, minimale Verzoegerung
    // nope, lowest speed, maximum delay
    set_repeat_rate (3, 30);
}

// KEY_HIT: Dient der Tastaturabfrage nach dem Auftreten einer Tastatur-
//          unterbrechung. Wenn der Tastendruck abgeschlossen ist und
//          ein Scancode, sowie gegebenenfalls ein ASCII Code emittelt
//          werden konnte, werden diese in Key zurueckgeliefert. Anderen-
//          falls liefert key_hit () einen ungueltigen Wert zurueck, was
//          mit Key::valid () ueberprueft werden kann.

Key Keyboard_Controller::key_hit()
{
    Key invalid;  // nicht explizit initialisierte Tasten sind ungueltig
    
    unsigned char status = static_cast<unsigned char>(ctrl_port.inb());
    
    //Check output buffer bit. If it's 1 then there's some data waiting to be read!
    if(status & outb)
    {
        //Read data
        code = (unsigned char) data_port.inb();
        
        //If auxb is set data comes from mouse, ignore it
        if(!(status & auxb))
        {
            //When the key is successfully decoded, return it
            //This is neccessary, otherwise there won't be any output
            if(key_decoded())
            {
                return gather;
            }
        }
    }
    return invalid;
}

// REBOOT: Fuehrt einen Neustart des Rechners durch. Ja, beim PC macht
//         das der Tastaturcontroller.

void Keyboard_Controller::reboot ()
{
    const IO_Port CMOS_ctrl(0x70);
    const IO_Port CMOS_data(0x71);
    const IO_Port system_control_portA(0x92);
//		DBG << "rebooting smp" << endl;
    CMOS_ctrl.outb(0xe + 1);
    CMOS_data.outb(0);
    system_control_portA.outb(0x3);
}
// SET_REPEAT_RATE: Funktion zum Einstellen der Wiederholungsrate der
//                  Tastatur. delay bestimmt, wie lange eine Taste ge-
//                  drueckt werden muss, bevor die Wiederholung einsetzt.
//                  Erlaubt sind Werte zwischen 0 (minimale Wartezeit)
//                  und 3 (maximale Wartezeit). speed bestimmt, wie
//                  schnell die Tastencodes aufeinander folgen soll.
//                  Erlaubt sind Werte zwischen 0 (sehr schnell) und 31
//                  (sehr langsam).

void Keyboard_Controller::set_repeat_rate (int speed, int delay)
{
    //Wait until the input buffer of the keyboard controller is empty, so a new command can be written to it
    //Even if we are writing data to the keyboard encoder data still passes the keyboard controller first, so we
    //have to wait for it to be ready before we can pass another command.
    while(ctrl_port.inb()&inpb)
    {
    }
    
    //ctrl_port.outb(0xf3); There are definitions in the header...
    data_port.outb(kbd_cmd::set_speed);
    wait_for_ack(data_port);
    data_port.outb(delay<<5|speed);
    wait_for_ack(data_port);
}

void Keyboard_Controller::drainKeyboardBuffer()
{
    unsigned char status = static_cast<unsigned char>(ctrl_port.inb());
    
    //Read buffer until it's empty
    while(status & outb)
    {
        //Read data
        data_port.inb();
        status = static_cast<unsigned char>(ctrl_port.inb());
    }
}

//Waits until the keyboard controller returns an ACK
void Keyboard_Controller::wait_for_ack(IO_Port port)
{
    while(port.inb()!=kbd_reply::ack)
    {
    }
}

// SET_LED: setzt oder loescht die angegebene Leuchtdiode

void Keyboard_Controller::set_led (led_t led, bool on)
{
    //Read old port values
    uint8_t old_port = data_port.inb();

    //Wait for control port
    while(ctrl_port.inb()&inpb)
    {
    }

    data_port.outb(kbd_cmd::set_led);
    wait_for_ack(data_port);
    //Write new values
    if(on)
    {
        data_port.outb(old_port|led);
    }
    else
    {
        data_port.outb(old_port&led);
    }
}
