#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "jaglib.h"

static char jag_stdin_buffer[BUFSIZ]; // skunkCONSOLEREAD enforce "even" addresses
extern uint8_t _skunk_file_inuse;

size_t jagread(int h,char *p,size_t l)
{
    size_t reslength=jag_io_read(h,p,l); // -1=EOF <-1 is ERROR
    if (reslength<EOF)
    {
        if (skunkConsoleUp && h==stdin->filehandle)
        {
            memset(jag_stdin_buffer,0,BUFSIZ); // Clear to find EoS later
            char *c=skunkCONSOLEREAD(jag_stdin_buffer,BUFSIZ-2);
            while (c>=jag_stdin_buffer && (*c==0 || isspace(*c))) --c; // find EoS
            *++c='\n'; // force CR
            *++c=0; // terminate
            strcpy(p,jag_stdin_buffer); // Copy with termination 
            return c-jag_stdin_buffer; // Calc length
        }
        if (skunkConsoleUp && _skunk_file_inuse && h==4)
        {
            size_t c=skunkFILEREAD(jag_stdin_buffer,l);
            if (c==0) return EOF;
            memcpy(p,jag_stdin_buffer,c);
            return c;
        }
        return -2;
    }
    return reslength;
}
