        include "jaguar.inc"
        include "68kmacros.inc"

        xdef _jag_memset8

        text
_jag_memset8: ;void jag_memset8(void *dest,uint16_t repcount,uint16_t count,uint8_t value)
        jsr     _jag_wait_blitter_ready
        move.l  4(sp),d0
        phrase_align d0,d1
        move.l	d0,A1_BASE		; dest
        move.l	d1,A1_PIXEL
        move.l	#PIXEL8|XADDPIX|PITCH1,A1_FLAGS
        move.l	16(SP),B_PATD		; value
        move.w  10(sp),d0               ; repcount
        swap    d0
        move.w  14(sp),d0               ; count
        move.l	d0,B_COUNT
        move.l	#PATDSEL,B_CMD
        rts
