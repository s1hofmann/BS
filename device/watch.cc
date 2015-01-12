// vim: set et ts=4 sw=4:

#include "device/watch.h"
#include "machine/plugbox.h"

extern Plugbox plugbox;

Watch::Watch()
{
}

Watch::~Watch()
{
}

bool Watch::windup(uint32_t us)
{
    plugbox.assign(Plugbox::timer, this);
    this->interval_ = us;

    return true;
}

bool Watch::prologue()
{
    return true;
}

void Watch::epilogue()
{
}

uint32_t Watch::interval()
{
    return this->interval_;
}

void Watch::activate()
{
}
