#include <stdint.h>
#include "jagcore.h"

uint16_t _global_stick_mask=0x100;

void jag_audio_mute(uint16_t mute)
{
	_global_stick_mask=mute?0x00:0x100;
	*JOYSTICK=0x8000|_global_stick_mask;
}
