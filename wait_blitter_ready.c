#include "jagcore.h"

void jag_wait_blitter_ready()
{
    while (!(*B_CMD&1));
}