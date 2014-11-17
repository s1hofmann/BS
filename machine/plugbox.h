// vim: set et ts=4 sw=4
#include "guard/gate.h"

class Plugbox {

public:

    enum Vector { timer = 32, keyboard = 33, assassin = 100, wakeup = 101 };
    // Initialisierung der Gate map mit einem Default Gate.
    Plugbox ();
    // Einstöpseln einer Behandlungsroutine,
    // die in Form eines Gate-Objekts vorhanden sein muss.
    void assign (unsigned int vector, Gate *gate);
    // Abfrage eines eingetragenen Gate Objekts.
    Gate* report (unsigned int vector);

private:

    Gate* gate_map[256];
};
