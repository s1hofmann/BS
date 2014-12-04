#ifndef KEYBOARD_H_MWKYFDLH
#define KEYBOARD_H_MWKYFDLH

#include "machine/ioapic.h"
#include "machine/plugbox.h"
#include "machine/keyctrl.h"
#include "machine/apicsystem.h"

class Keyboard : public Gate, public Keyboard_Controller
{
public:
    Keyboard();
    ~Keyboard();

    void plugin();
<<<<<<< HEAD
    
    virtual bool prologue();
    virtual void epilogue();
=======
    virtual bool prologue();
    virtual void epilogue();
//    virtual void trigger();
>>>>>>> testing_marcel_a3

private:
    /* data */
    Key k;

};

#endif /* end of include guard: KEYBOARD_H_MWKYFDLH */
