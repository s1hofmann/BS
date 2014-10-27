
// vim: set et ts=4 sw=4:

#include "machine/cgascr.h"
#include "machine/io_port.h"

char *const CGA_Screen::CGA_START = reinterpret_cast<char *const>(0xb8000);

CGA_Screen::CGA_Screen(int from_col, int to_col, int from_row, int to_row, bool use_cursor)
{
    this->scr_fx = from_col;
    this->scr_fy = from_row;
    this->scr_tx = to_col;
    this->scr_ty = to_row;

    this->scr_cursor = use_cursor;

    setpos(0, 0);

    cls(' ');

    drawFrame();
}

void CGA_Screen::drawFrame()
{
    for(int x = 0; x < this->scr_tx - this->scr_fx; ++x)
    {
        show(x, this->scr_ty - this->scr_fy, '-');
    }

    for(int y = 0; y < this->scr_ty - this->scr_fy; ++y)
    {
        show(this->scr_tx - this->scr_fx, y, '|');
    }
}

unsigned char CGA_Screen::attribute(CGA_Screen::color bg, CGA_Screen::color fg, bool blink)
{
    unsigned char scr_attribute = blink << 8 | bg << 4 | fg;
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

        //No memory addresses needed here, just the absolute offset from top left corner
        int offset = (this->pos_x+this->scr_fx) + (this->pos_y+this->scr_fy)*80;
        scr_index.outb(15);
        scr_data.outb(offset);
        scr_index.outb(14);
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
    if((x >= 0 and y >= 0) and (x <= this->scr_tx - this->scr_fx and y <= this->scr_ty - this->scr_fy))
    {
        CGA_Screen::CGA_START[getoffset(x+this->scr_fx, y+this->scr_fy)] = character; 
        if(attrib)
        {
            CGA_Screen::CGA_START[getoffset(x+this->scr_fx, y+this->scr_fy)+1] = attrib;
        }
        else
        {
            CGA_Screen::CGA_START[getoffset(x+this->scr_fx, y+this->scr_fy)+1] = CGA_Screen::STD_ATTR;
        }
    }
}

void CGA_Screen::print(char *string, int length, unsigned char attrib)
{
    int i = 0;
    while(i < length)
    {
        //Newline
        if(string[i] == '\n')
        {
            setpos(0, this->pos_y+1);
        }
        else
        {
            show(this->pos_x, this->pos_y, string[i], attrib);
            setpos(this->pos_x+1, this->pos_y);
        }
        //Word wrap
        if(this->pos_x+1 > (this->scr_tx - this->scr_fx))
        {
            setpos(0, this->pos_y+1);
        }
        ++i;
    }

    drawFrame();
}

void CGA_Screen::cls(char x)
{
    for(int i = this->scr_fx; i < this->scr_tx; ++i)
    {
        for(int j = this->scr_fy; j < this->scr_ty; ++j)
        {
            CGA_Screen::CGA_START[getoffset(i, j)] = x;
            CGA_Screen::CGA_START[getoffset(i, j)+1] = STD_ATTR;
        }
    }
}
