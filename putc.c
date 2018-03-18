#include <stdint.h>
#include <stdio.h>
#include "jaglib.h"

static uint16_t _posx=0;
static uint8_t _posy=0;

void jag_blit_char(uint8_t *src,uint16_t offset, uint8_t color_index);
extern uint8_t jag_xlfont[];

void jag_putc(char ch,uint8_t color_index)
{
	switch (ch)
	{
	case '\n':
		_posx = 0;
		_posy += 8;
		return;
	case 0x13:
		_posx = 0;
		return;
	case 0x10:
		_posy += 8;
		return;
	case 0x00:
		return;
	}
	if (_posx > CONSOLE_BMP_WIDTH - 8)
	{
		_posx = 0;
		_posy += 8;
	}
	while (_posy > CONSOLE_BMP_HEIGHT - 8)
	{
		_posy -= 8;
		jag_memcpy32p(jag_vidmem, jag_vidmem + 8 * CONSOLE_BMP_WIDTH, CONSOLE_BMP_WIDTH, (CONSOLE_BMP_HEIGHT - 8)/4);
		jag_memset32(jag_vidmem+CONSOLE_BMP_WIDTH*(CONSOLE_BMP_HEIGHT - 8),8,CONSOLE_BMP_WIDTH/4,JAG_CONSOLE_BACKGROUND_COLORINDEX32);
	}
	jag_blit_char(jag_xlfont + ((ch & 0x7f) << 3), (_posy << 16) | _posx,color_index);
	_posx += 8;
}

void jag_console_clear()
{
	jag_memset32(jag_vidmem,CONSOLE_BMP_HEIGHT,CONSOLE_BMP_WIDTH/4,JAG_CONSOLE_BACKGROUND_COLORINDEX32);
	_posx=_posy=0;
}

void jag_console_set_cursor(uint16_t x,uint8_t y)
{
	fflush(stdout);
	_posx=x;
	_posy=y;
}