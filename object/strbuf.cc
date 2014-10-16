
// vim: set et ts=4 sw=4:

#include "object/strbuf.h"

//extern "C" void __cxa_pure_virtual() { while (1); }

//void operator delete(void*) {};
//void operator delete[] (void*) {};

Stringbuffer::Stringbuffer()
{
    for(int i = 0; i < 80; ++i)
    {
        this->buffer[i] = 0;
    }

    this->pos = 0;
}

void Stringbuffer::put(char c)
{
    if(this->pos+1 < 79)
    {
        this->buffer[pos] = c;
        ++this->pos;
    }
}
