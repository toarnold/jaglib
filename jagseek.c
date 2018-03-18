#include "jaglib.h"

long jagseek(int h,long offset,int direction)
{
    long c=jag_io_seek(h,offset,direction);
    if (c==-1)
    {
        return -1; // not supported
    }
    return c;
}
