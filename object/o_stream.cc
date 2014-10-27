
// vim: set et ts=4 sw=4:

#include "object/o_stream.h"

O_Stream::O_Stream()
{
    this->base = 10;
}

O_Stream::~O_Stream()
{
}

O_Stream& O_Stream::operator<<(char c) //No conversion needed, simply push the value to the mem
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

    while(string[i] != '\0')
    {
        this->put(string[i]);
        ++i;
    }
    return *this;
}
        
O_Stream& O_Stream::operator<<(bool b)
{
    char ctrue[4] = {'t', 'r', 'u', 'e'};
    char cfalse[5] = {'f', 'a', 'l', 's', 'e'};

    if(b == true)
    {
        for (int i=0; i<4; ++i)
        {
            this->put(ctrue[i]);
        }
    }
    else if(b == false)
    {
        for (int i=0; i<5; ++i)
        {
            this->put(cfalse[i]);
        }
    }

    return *this;
}

O_Stream& O_Stream::operator<<(short ival)
{
    *this << static_cast<long>(ival);
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned short ival)
{
    *this << static_cast<unsigned long>(ival);
    return *this;
}

O_Stream& O_Stream::operator<<(int ival)
{
    *this << static_cast<long>(ival);
    return *this;
}

O_Stream& O_Stream::operator<<(unsigned int ival)
{
    *this << static_cast<unsigned long>(ival);
    return *this;
}

O_Stream& O_Stream::operator<<(long ival)
{
    int len = 0, pos = 1;
    //assert(sizeof(long) <= 8);
    char out[20]; // 20 -> max digits for output
    long tmp = ival;
    
    if(this->base == 8)
    {
        this->put('0');
    }
    else if(this->base == 16)
    {
        this->put('0');
        this->put('x');
    }
    else if(this->base == 2)
    {
        this->put('0');
        this->put('b');
    }
    else
    {
        if(ival < 0)
        {
            this->put('-');
        }
    }

    //Calculate length
    do
    {
        ++len;
        tmp/=this->base;
    } while(tmp); //The lazy man's guide to logarithm

    //char out[len];

    //Convert
    int idx = 0;
    do
    {
        idx = (ival%this->base);
        if(idx<0) idx*=-1;
        out[len-pos] = this->nums[idx];
        ival/=this->base;
        ++pos;
    } while(ival);


    for(int i=0; i<len; ++i)
    {
        this->put(out[i]);
    }

    return *this;
}

O_Stream& O_Stream::operator<<(unsigned long ival)
{
    int len = 0, pos = 1;
    //assert(sizeof(long) < 8);
    char out[20];  // 20 -> max digits for output
    unsigned long tmp = ival;

    //Calculate length
    do
    {
        ++len;
        tmp/=this->base;
    } while(tmp); //The lazy man's guide to logarithm

    //char out[len];

    //Convert
    do
    {
        out[len-pos] = this->nums[ival%this->base];
        ival/=this->base;
        ++pos;
    } while(ival);

    if(this->base == 8)
    {
        this->put('0');
    }
    else if(this->base == 16)
    {
        this->put('0');
        this->put('x');
    }
    else if(this->base == 2)
    {
        this->put('0');
        this->put('b');
    }

    for(int i=0; i<len; ++i)
    {
        this->put(out[i]);
    }

    return *this;
}

O_Stream& O_Stream::operator<<(void *ptr)
{
    unsigned long tmp = reinterpret_cast<unsigned long>(ptr);
    *this << hex << tmp;
    return *this;
}

O_Stream& O_Stream::operator<<(O_Stream &(*f) (O_Stream&))
{
    return f(*this);
}

O_Stream &bin(O_Stream &stream)
{
    stream.base = 2;
    return stream;
}

O_Stream &oct(O_Stream &stream)
{
    stream.base = 8;
    return stream;
}

O_Stream &hex(O_Stream &stream)
{
    stream.base = 16;
    return stream;
}

O_Stream &dec(O_Stream &stream)
{
    stream.base = 10;
    return stream;
}

O_Stream &endl(O_Stream &stream) //Add null terminator and flush the stream to output
{
    stream << '\n';
    stream.flush();
    return stream;
}
