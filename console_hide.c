#include "jaglib.h"

void jag_console_hide()
{
    jag_attach_olp((void *)((uint32_t)jag_console_bmp->p0.link<<3)); // appended or stop object
}
