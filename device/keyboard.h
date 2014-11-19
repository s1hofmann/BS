// vim: set et ts=4 sw=4
#include "guard/gate.h"
#include "machine/keyctrl.h"
#include "machine/key.h"

class Keyboard : public Keyboard_Controller, public Gate {

public:
    Keyboard();
    ~Keyboard();
    // Anstöpseln' der Tastatur.
    void plugin();
    // Unterbrechungsbehandlungsroutine
    virtual void trigger();
private:
    Key k;
};
