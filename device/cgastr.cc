
// vim: set et ts=4 sw=4:

#include "device/cgastr.h"

CGA_Stream::CGA_Stream(int from_col, int to_col, int from_row, int to_row, bool use_cursor) : CGA_Screen(from_col, to_col, from_row, to_row, use_cursor), O_Stream()
{
    this->attr = CGA_Screen::STD_ATTR;
    this->cls(' ');
}

CGA_Stream::~CGA_Stream()
{
}

void CGA_Stream::flush()
{
    this->print(this->buffer, this->pos, this->attr);
    this->pos = 0;
}
