#include <stdint.h>
#include "jagcore.h"

void jag_set_indexed_color(uint16_t index,uint16_t color)
{
	*(CLUT + index) = color;
}