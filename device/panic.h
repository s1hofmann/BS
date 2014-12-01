#ifndef PANIC_H
#define PANIC_H

#include "guard/gate.h"

class Panic : public Gate
{
public:

    bool prologue();

private:
    /* data */
};
#endif
