#ifndef PLUGBOX_H
#define PLUGBOX_H 

#include "device/panic.h"

class Plugbox
{
public:
    Plugbox ();
    virtual ~Plugbox ();
    void assign(unsigned int vector, Gate *gate);
    Gate* report(unsigned int vector);
    
    enum Vector { timer=32, keyboard=33, assassin=100, wakeup=101 };

private:
    /* data */

    //Plugbox resembles the interrupt redirection table, so we need 256 possible slots.
    Gate *gate[256];
};
#endif
