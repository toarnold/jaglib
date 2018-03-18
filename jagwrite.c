#include <stdio.h>
#include <string.h>
#include "jaglib.h"

static char jag_write_buffer[BUFSIZ+2];
void jag_putc(char ch,uint8_t color_index);
extern uint8_t _skunk_file_inuse;

size_t jagwrite(int h,const char *p, size_t l)
{
    size_t c=jag_io_write(h,p,l);
    if (c==-1)
    {
        if (h==stderr->filehandle && skunkConsoleUp)
        {
            uint8_t terminate;
            c=l;
            do {
                terminate=l<=BUFSIZ;
                uint16_t size=terminate?l:BUFSIZ;
                memcpy(jag_write_buffer,p,size);
                jag_write_buffer[size]=0; // terminate
                skunkCONSOLEWRITE(jag_write_buffer);
                l-=BUFSIZ;
                p+=size;
            } while (!terminate);
            return c;
        }
        if (h==stdout->filehandle || (h==stderr->filehandle && !skunkConsoleUp)) // stdout or stderr
        {
            for (int index=0;index<l;++index)
            {
                jag_putc(p[index],JAG_CONSOLE_BACKGROUND_COLORINDEX+h);
            }
            return l;
        }
        if (skunkConsoleUp && _skunk_file_inuse && h==4)
        {
            uint8_t terminate;
            c=l;
            do {
                terminate=l<=BUFSIZ;
                uint16_t size=terminate?l:BUFSIZ;
                memcpy(jag_write_buffer,p,size);
                jag_write_buffer[size]=0; // make even
                skunkFILEWRITE(jag_write_buffer,size+(size&1));
                l-=BUFSIZ;
                p+=size;
            } while (!terminate);
        }
    }
	return c;
}
