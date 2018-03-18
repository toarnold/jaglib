#include <stdint.h>
#include "jagcore.h"

extern uint16_t _global_stick_mask;

/* RLDUAP147*B2580C369#O */
uint32_t jag_read_stick1(uint16_t readdirectionsonly)
{
	*JOYSTICK=0x8070 | _global_stick_mask;
	uint32_t result=(*JOYSTICK>>12^0x0f)&0x0f;
	result<<=2;
	result|=(*JOYBUTS>>2^0x03)&0x03;

	if (readdirectionsonly)
		return result<<15;
	
	*JOYSTICK= 0x80b0 | _global_stick_mask;
	result<<=4;
	result|=(*JOYSTICK>>12^0x0f)&0x0f;
	result<<=1;
	result|=(*JOYBUTS>>3^0x01)&0x01;

	*JOYSTICK= 0x80d0 | _global_stick_mask;
	result<<=4;
	result|=(*JOYSTICK>>12^0x0f)&0x0f;
	result<<=1;
	result|=(*JOYBUTS>>3^0x01)&0x01;

	*JOYSTICK=0x80e0 | _global_stick_mask;
	result<<=4;
	result|=(*JOYSTICK>>12^0x0f)&0x0f;
	result<<=1;
	result|=(*JOYBUTS>>3^0x01)&0x01;

	return result;
}
