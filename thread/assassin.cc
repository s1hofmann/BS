// vim: set et ts=4 sw=4:

#include "thread/assassin.h"
#include "machine/plugbox.h"

extern Plugbox plugbox;

void Assassin::hire()
{
    plugbox.assign(Plugbox::assassin, this);
}

void Assassin::epilogue()
{
}
