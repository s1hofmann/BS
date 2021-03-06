#ifndef PANIC_H
#define PANIC_H

#include "guard/gate.h"

class Panic : public Gate
{
public:
    Panic();
    ~Panic();

    virtual bool prologue();
    virtual void epilogue();

private:
    /* data */
};
#endif
