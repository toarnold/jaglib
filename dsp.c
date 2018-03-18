#include "jaglib.h"
#include "jagcore.h"

void jag_dsp_load(void *loadadr,const void *codestartadr, uint16_t count)
{
	count=(count+3)&~3; // Calc long words
	jag_memcpy32p(loadadr,codestartadr,1,count);
	jag_wait_blitter_ready();
}

uint8_t jag_dsp_is_running()
{
	return *D_CTRL & RISCGO;
}

uint8_t jag_dsp_go(const void *dspstartadr, uint16_t addFlags)
{
	if (jag_dsp_is_running())
		return 0;

	*D_PC = (uint32_t)dspstartadr;
	*D_CTRL = addFlags | RISCGO;
	return 1;
}

void jag_dsp_wait()
{
	while (jag_dsp_is_running());
}
