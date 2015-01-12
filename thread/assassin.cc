// vim: set et ts=4 sw=4:

#include "thread/assassin.h"
#include "main.cc"

extern Plugbox plugbox;

void Assassin::hire()
{
    plugbox.assign(Plugbox::assassin, this);
}

virtual void Assassin::epilogue()
{
}
