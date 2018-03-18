#include <stdint.h>
#include "jagcore.h"
#include "jaglib.h"

static volatile uint8_t jag_vbl_occurs=0;
extern uint32_t _global_olp;
extern uint32_t _attach_olp;
extern uint32_t _append_olp;

static uint16_t _global_int_mask=C_VIDENA;

void jag_set_cpu_int_mask(uint16_t mask)
{
	_global_int_mask=mask;
	*INT1=mask;
}

__interrupt void jag_global_interrupt_handler()
{
	uint16_t setint1 = 0xff00 & jag_custom_interrupt_handler();
	if (*INT1&C_VIDENA)
	{
		setint1 |= C_VIDCLR;
        jag_console_bmp->p0.height = CONSOLE_BMP_HEIGHT;
        jag_console_bmp->p0.data = (uint32_t)jag_vidmem >> 3;

		if (_global_olp)
		{
			*OLP=_global_olp;
			_global_olp=0;
		}
		if (_attach_olp)
		{
			jag_logical_root->link=_attach_olp >> 3;
			_attach_olp=0;
		}
		if (_append_olp)
		{
			jag_console_bmp->p0.link=_append_olp >> 3;
			_append_olp=0;
		}
		jag_vbl_occurs=1;
	}
	*INT1 =_global_int_mask | setint1;
	*INT2 = 0;
}

void jag_wait_vbl()
{
	jag_vbl_occurs=0;
	while(!jag_vbl_occurs);
}
