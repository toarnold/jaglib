#include <stdint.h>
#include "jagcore.h"

extern uint16_t _global_stick_mask;

/* RLDUAP147*B2580C369#O */
uint32_t jag_read_stick0(uint16_t readdirectionsonly)
{
	*JOYSTICK= 0x800e | _global_stick_mask;
	uint32_t result=(*JOYSTICK>>8^0x0f)&0x0f;
	result<<=2;
	result|=(*JOYBUTS^0x03)&0x03;

	if (readdirectionsonly)
		return result<<15;
	
	*JOYSTICK= 0x800d | _global_stick_mask;
	result<<=4;
	result|=(*JOYSTICK>>8^0x0f)&0x0f;
	result<<=1;
	result|=(*JOYBUTS>>1^0x01)&0x01;

	*JOYSTICK= 0x800b | _global_stick_mask;
	result<<=4;
	result|=(*JOYSTICK>>8^0x0f)&0x0f;
	result<<=1;
	result|=(*JOYBUTS>>1^0x01)&0x01;

	*JOYSTICK= 0x8007 | _global_stick_mask;
	result<<=4;
	result|=(*JOYSTICK>>8^0x0f)&0x0f;
	result<<=1;
	result|=(*JOYBUTS>>1^0x01)&0x01;

	return result;
}
