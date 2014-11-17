// vim: set et ts=4 sw=4
#include "guard/gate.h"

class Panic : public Gate {

public:
    // Einfachste Form der Unterbrechungsbehandlung:
    // Es wird eine Fehlermeldung ausgegeben und der Prozessor angehalten.
    void trigger();
};
