#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "jaglib.h"
#include "jagcore.h"

uint16_t jag_hdb;
uint16_t jag_hde;
uint16_t jag_vdb;
uint16_t jag_vde;
uint16_t jag_width;
uint16_t jag_height;

uint64_t *jag_listbuf;
op_bmp_object *jag_console_bmp;
op_branch_object *jag_logical_root;
uint8_t *jag_vidmem;
void *_append_olp;
void *_attach_olp;
void *_global_olp;
uint8_t _skunk_file_inuse;

__interrupt void jag_global_interrupt_handler();
extern uint16_t jag_console_hide_startup;
void jag_welcome_message();
void jag_init_message();
void jag_initdebug();

static void init_console()
{
	// THE Bitmap
    jag_vidmem = malloc(CONSOLE_BMP_WIDTH*CONSOLE_BMP_HEIGHT);
	jag_set_indexed_color(JAG_CONSOLE_BACKGROUND_COLORINDEX, toRgb16(0x00, 0x00, 0xff)); // ATARI blue
	jag_set_indexed_color(JAG_CONSOLE_STDOUT_COLORINDEX, toRgb16(0xff, 0xff, 0xff)); // Font-Color white
	jag_set_indexed_color(JAG_CONSOLE_STDERR_COLORINDEX, toRgb16(0xff, 0x00, 0x00)); // Font-Color red
	jag_console_clear();

	jag_listbuf = calloc(4,8);
    op_branch_object *br1=(op_branch_object *)&jag_listbuf[0];
    op_branch_object *br2=(op_branch_object *)&jag_listbuf[1];
	jag_logical_root = (op_branch_object *)&jag_listbuf[2];
	op_stop_object *stop = (op_stop_object *)&jag_listbuf[3];
	uint32_t stopadress = (uint32_t)stop >> 3;
	jag_console_bmp=calloc(1,sizeof(op_bmp_object));

	//0
	br1->type = BRANCHOBJ;
	br1->cc = O_BRLT >> 14;
	br1->ypos = jag_vde;
	br1->link = stopadress;

	//1
	br2->type = BRANCHOBJ;
	br2->cc = O_BRGT >> 14;
	br2->ypos = jag_vdb;
	br2->link = stopadress;

	//2
	jag_logical_root->type = BRANCHOBJ;
	jag_logical_root->cc = O_BRGT >> 14;
	jag_logical_root->ypos = 0x7ff;		// springe immer ...
	jag_logical_root->link = jag_console_hide_startup?stopadress:(uint32_t)jag_console_bmp >> 3; 

	// 3 Stop object
	stop->type = STOPOBJ;
	stop->int_flag = 1;

	jag_console_bmp->p0.type = BITOBJ;
	jag_console_bmp->p0.ypos = (jag_height - CONSOLE_BMP_HEIGHT + jag_vdb) & 0xfffe;
	jag_console_bmp->p0.height = CONSOLE_BMP_HEIGHT;
	jag_console_bmp->p0.link = stopadress;
	jag_console_bmp->p0.data = (uint32_t)jag_vidmem >> 3;
	jag_console_bmp->p1.xpos = (jag_width / 4 - CONSOLE_BMP_WIDTH) / 2;
	jag_console_bmp->p1.depth = O_DEPTH8 >> 12;
	jag_console_bmp->p1.pitch = 1;
	jag_console_bmp->p1.dwidth = jag_console_bmp->p1.iwidth = CONSOLE_BMP_WIDTH / 8;

	*V_AUTO=(uint32_t)jag_global_interrupt_handler;
	*VI = jag_vde | 1;
	jag_set_cpu_int_mask(C_VIDENA); // Enable video interrupts
	jag_set_sr(0x2100|(jag_get_sr()&0xff)); // Supervisor mode and Interruptlevel 1
	jag_set_olp(jag_listbuf);
	*VMODE=PWIDTH4|CSYNC|BGEN|RGB16|VIDEN;     	// Configure Video
}

static void init_video()
{
	uint16_t vmid;
	uint16_t hmid;

	if (*CONFIG & VIDTYPE)
	{
		jag_width = NTSC_WIDTH;
		jag_height = NTSC_HEIGHT;
		vmid = NTSC_VMID;
		hmid = NTSC_HMID;
	}
	else
	{
		jag_width = PAL_WIDTH;
		jag_height = PAL_HEIGHT;
		vmid = PAL_VMID;
		hmid = PAL_HMID;
	}
	*HDE = jag_hde = 0x0400 | (jag_width / 2 - 1);
	jag_hdb = hmid - jag_width / 2 + 4;
	*HDB1 = jag_hdb;
	*HDB2 = jag_hdb;
	*VDB = jag_vdb = vmid - jag_height;
	*VDE = 0xffff; // don't know why?
	jag_vde = vmid + jag_height;
	*BORD1 = 0; // Black border
	*BORD2 = 0; // Black border
	*BG = 0; // Init line buffer to black
}

void _INIT_3_video()
{
	jag_initdebug();
	init_video();
	init_console();
	jag_init_message();
	skunkRESET();
	jag_welcome_message();
}

void _EXIT_3_video()
{
	if (skunkConsoleUp)
	{
		skunkCONSOLECLOSE();
	}
}
