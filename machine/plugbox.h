#ifndef PLUGBOX_H
#define PLUGBOX_H 

enum Vector { timer=32, keyboard=33, assassin=100, wakeup=101 };

class Plugbox
{
public:
    Plugbox ();
    virtual ~Plugbox ();
    void assign(unsigned int vecotr, Gate *gate);
    Gate* report(unsigned int vector);

private:
    /* data */
};
#endif
