#include "jaglib.h"

extern uint64_t *jag_listbuf;
extern op_bmp_object *jag_console_bmp;

void jag_restore_ol()
{
	jag_console_show();
	jag_append_olp(&jag_listbuf[3]); // Stop object
}