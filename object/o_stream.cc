
// vim: set et ts=4 sw=4:

#include "object/o_stream.h"

O_Stream& O_Stream::operator<<(char c)
{
    this->put(c);
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned char c)
{
    this->put(c);
    return *this;
}
        
O_Stream& O_Stream::operator<<(char *string)
{
    int i = 0;

    while(string[i] != '0')
    {
        this->put(string[i]);
        ++i;
    }
    return *this;
}
        
O_Stream& O_Stream::operator<<(bool b)
{
    return *this;
}
        
O_Stream& O_Stream::operator<<(short ival)
{
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned short ival)
{
    return *this;
}

O_Stream& O_Stream::operator<<(int ival)
{
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned int ival)
{
    return *this;
}

O_Stream& O_Stream::operator<<(long ival)
{
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned long ival)
{
    return *this;
}

O_Stream& O_Stream::operator<<(void *ptr)
{
    return *this;
}
