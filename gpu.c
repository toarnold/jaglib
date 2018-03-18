#include "jaglib.h"
#include "jagcore.h"
#include <string.h>

void jag_gpu_load(void *loadadr,const void *codestartadr, uint16_t count)
{
	count=(count+3)&~3; // Calc long words
	jag_memcpy32p((void *)(((int)loadadr)|0x8000),codestartadr,1, count);
	jag_wait_blitter_ready();
}

uint8_t jag_gpu_is_running()
{
	return *G_CTRL & RISCGO;
}

uint8_t jag_gpu_go(const void *gpustartadr, uint16_t addFlags)
{
	if (jag_gpu_is_running())
		return 0;

	*G_PC = (uint32_t)gpustartadr;
	*G_CTRL = addFlags | RISCGO;
	return 1;
}

void jag_gpu_wait()
{
	while (jag_gpu_is_running());
}
