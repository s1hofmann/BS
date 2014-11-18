
// vim: set et ts=4 sw=4:

#include "object/strbuf.h"
#include "machine/spinlock.h"

extern Spinlock lock;

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
    lock.lock();
    if(this->pos+1 < 79)
    {
        this->buffer[pos] = c;
        ++this->pos;
    }
    else
    {
        this->flush();
    }
    lock.unlock();
}
