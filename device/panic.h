#ifndef PANIC_H
#define PANIC_H

#include "guard/gate.h"

class Panic : public Gate
{
public:
    Panic ();
    virtual ~Panic ();

    bool prologue();
    void epilogue();

private:
    /* data */
};
#endif
