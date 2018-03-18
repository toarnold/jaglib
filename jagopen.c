#include <stdio.h>
#include <string.h>
#include "jaglib.h"

extern uint8_t _skunk_file_inuse;

int jagopen(const char *name,const char *mode)
{
    int c=jag_io_open(name,mode);
    if (c==-1)
    {
        if (skunkConsoleUp && !_skunk_file_inuse)
        {
            if (strcmp(mode,"r") && strcmp(mode,"w"))
                return -1; // Not supported
            skunkFILEOPEN(name,strcmp(mode,"r")==0?1:0);
            _skunk_file_inuse=1;
            return 4;
        }
    }
    return c;
}