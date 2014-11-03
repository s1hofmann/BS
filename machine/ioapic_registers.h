// vim: set et ts=4 sw=4


struct IOREDTBL_L {
    unsigned int vector:8;
    unsigned int delivery_mode:3; 
    unsigned int destination_mode:1;
    unsigned int delivery_status:1; 
    unsigned int polarity:1;
    unsigned int remote_irr:1;
    unsigned int trigger_mode:1;
    unsigned int mask:1; 
    unsigned int reserved:15;
}; 

struct IOREDTBL_H {
    unsigned int reserved:24;
    unsigned int logical_destination:8;
};
