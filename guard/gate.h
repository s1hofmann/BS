// vim: set et ts=4 sw=4
#ifndef GATE_H
#define GATE_H

class Gate {

public:
    Gate(){};
    virtual ~Gate(){};
    // Unterbrechungsbehandlungsroutine
    virtual void trigger() = 0;
};

#endif /* include guard for gate.h */
