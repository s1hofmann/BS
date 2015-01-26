#ifndef KEYBOARD_H_MWKYFDLH
#define KEYBOARD_H_MWKYFDLH

#include "machine/ioapic.h"
#include "machine/plugbox.h"
#include "machine/keyctrl.h"
#include "machine/apicsystem.h"

#include "meeting/semaphore.h"

class Keyboard : public Gate, public Keyboard_Controller
{
public:
    Keyboard();
    ~Keyboard();

    void plugin();
    
    virtual bool prologue();
    virtual void epilogue();
    Key getkey();

private:
    /* data */
    Key k;
    Semaphore s;

};

#endif /* end of include guard: KEYBOARD_H_MWKYFDLH */
