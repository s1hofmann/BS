
// vim: set et ts=4 sw=4:

#include "machine/cgascr.h"
#include "machine/io_port.h"

char *const CGA_Screen::CGA_START = reinterpret_cast<char *const>(0xb8000);

CGA_Screen::CGA_Screen(int from_col, int to_col, int from_row, int to_row, bool use_cursor)
{
    this->scr_x = from_col;
    this->scr_y = from_row;
    this->scr_width = to_col - from_col;
    this->scr_height = to_row - from_row;

    this->scr_cursor = use_cursor;
}

unsigned char CGA_Screen::attribute(CGA_Screen::color bg, CGA_Screen::color fg, bool blink)
{
    unsigned char scr_attribute = blink << 7 | bg << 4 | fg;
    return scr_attribute;
}

void CGA_Screen::setpos(int x, int y)
{
    this->pos_x = x;
    this->pos_y = y;

    if(this->scr_cursor)
    {
        IO_Port scr_index(0x3d4);
        IO_Port scr_data(0x3d5);

        int offset = *CGA_START+getoffset(x, y);
        scr_index.outb(14);
        //write low byte
        scr_data.outb(offset & 0xff);
        scr_index.outb(15);
        //shift and write high byte
        scr_data.outb(offset >> 8);
    }
}

int CGA_Screen::getoffset(int x, int y)
{
    return (x*2)+(y*2*COLUMNS);
}

void CGA_Screen::getpos(int &x, int &y)
{
    x = this->pos_x;
    y = this->pos_y;
}

void CGA_Screen::show(int x, int y, char character, unsigned char attrib)
{
    if((x >= 0 and y >= 0) and (x < COLUMNS and y < ROWS))
    {
        CGA_Screen::CGA_START[getoffset(x, y)] = character; 
        if(attrib)
        {
            CGA_Screen::CGA_START[getoffset(x, y)+1] = attrib;
        }
        else
        {
            CGA_Screen::CGA_START[getoffset(x, y)+1] = CGA_Screen::STD_ATTR;
        }
    }

    setpos(this->pos_x + 1, this->pos_y);
}

void CGA_Screen::print(char *string, int length, unsigned char attrib)
{
    for(int i = 0; i < length; ++i)
    {
        show(this->pos_x, this->pos_y, string[i], attrib);
    }
}

void CGA_Screen::cls()
{
    for(int i = 0; i < COLUMNS; ++i)
    {
        for(int j = 0; j < ROWS; ++j)
        {
            CGA_Screen::CGA_START[getoffset(i, j)] = ' ';
            CGA_Screen::CGA_START[getoffset(i, j)+1] = STD_ATTR;
        }
    }
}
