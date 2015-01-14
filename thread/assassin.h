// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Assassin
 */

#ifndef __assassin_h__
#define __assassin_h__

#include "guard/gate.h"

class Assassin : public Gate
{
public:
    Assassin() {};
    ~Assassin() {};

    void hire();
    bool prologue() { return true; }
    virtual void epilogue();

private:
    /* data */
};
#endif
