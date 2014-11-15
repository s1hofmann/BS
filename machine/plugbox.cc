#include "plugbox.h"
#include "guard/gate.h"

Plugbox::Plugbox()
{
    for(int i=0; i<256; ++i)
    {
        Plugbox::gate[i] = &panic;
    }
}

Plugbox::~Plugbox()
{
}

void Plugbox::assign(unsigned int vector, Gate *gate)
{
    Plugbox::gate[vector] = gate;
}

Gate* Plugbox::report(unsigned int vector)
{
    return Plugbox::gate[vector];
}
