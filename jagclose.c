#include "jaglib.h"

extern uint8_t _skunk_file_inuse;

void jagclose(int h)
{
    if (!jag_io_close(h))
    {
        if (skunkConsoleUp && _skunk_file_inuse && h==4)
        {
            skunkFILECLOSE();
            _skunk_file_inuse=0;
        }
    }
}