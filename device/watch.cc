// vim: set et ts=4 sw=4:

#include "device/watch.h"

Watch::Watch()
{
}

Watch::~Watch()
{
}

bool Watch::windup(uint32_t us)
{
    this->interval_ = us;
}

bool Watch::prologue()
{
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
