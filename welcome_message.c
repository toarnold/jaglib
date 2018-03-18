#include <stdio.h>
#include <stdint.h>
#include "jagcore.h"
#include "jaglib.h"

static void repeat(char c,uint16_t count)
{
	while (count--)
		putchar(c);
}

void jag_welcome_message()
{
	jag_console_set_cursor(0,0);
	putchar(0x11); repeat(0x12,38); putchar(0x05);
	putchar(0x7c); repeat(' ',38); putchar(0x7c);
	putchar(0x01); repeat(0x12,25); putchar(0x17); repeat(0x12,12); putchar(0x04);
	for (int i=0;i<5;++i)
	{
		putchar(0x7c); repeat(' ',25); putchar(0x7c); repeat(' ',12); putchar(0x7c);
	}
	putchar(0x1a); repeat(0x12,25); putchar(0x18); repeat(0x12,12); putchar(0x03);
	jag_console_set_cursor(8,8);
	puts("jaglib (" __DATE__ ") by toarnold");
	jag_console_set_cursor(8,24);
	puts("skunkboard console");
	jag_console_set_cursor(216,24);
	puts(skunkConsoleUp?"present":"no");
	jag_console_set_cursor(8,32);
	puts("stdout");
	jag_console_set_cursor(216,32);
	puts("console");
	jag_console_set_cursor(8,40);
	puts("stderr");
	jag_console_set_cursor(216,40);
	puts(skunkConsoleUp?"skunkboard":"console/red");
	jag_console_set_cursor(8,48);
	puts("stdin");
	jag_console_set_cursor(216,48);
	puts(skunkConsoleUp?"skunkboard":"disabled");
	jag_console_set_cursor(8,56);
	puts("video refresh rate");
	jag_console_set_cursor(216,56);
	puts(*CONFIG&VIDTYPE?"60Hz":"50Hz");
	jag_console_set_cursor(0,80);
}
