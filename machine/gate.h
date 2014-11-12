#ifndef GATE_H
#define GATE_H 

class Gate
{
public:
    Gate();
    virtual ~Gate();
    
    virtual void trigger()=0;
private:
    /* data */
};
#endif
