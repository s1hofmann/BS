// vim: set et ts=4 sw4
#include "plugbox.h"
#include "device/panic.h"
#define DEBUG 1
#include "object/debug.h"
extern Panic panic;
// global object definition
Plugbox plugbox;

Plugbox::Plugbox(){
    for(unsigned int i = 0; i < (sizeof(gate_map)/sizeof(gate_map[0])); i++){
        gate_map[i] = &panic;
    }
}

void Plugbox::assign (unsigned int vector, Gate *gate){
    DBG << "plugbox assing" << endl;
    DBG << "vector: " << vector << endl;
    gate_map[vector] = gate;
}

Gate* Plugbox::report (unsigned int vector){
    return gate_map[vector];
}
