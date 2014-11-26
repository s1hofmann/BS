#ifndef GATE_H_E0VAPSZB
#define GATE_H_E0VAPSZB

class Gate : public QueueEntry<Gate>
{
public:
    Gate() {};
    virtual ~Gate() {};
    virtual bool prologue()=0;
    virtual void epilogue();
    void set_queued(){
        // TBD
    }
    bool set_dequeued(){
        // TBD
    }
private:
    /* data */
};

#endif /* end of include guard: GATE_H_E0VAPSZB */
