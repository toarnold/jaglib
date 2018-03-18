        include "jaguar.inc"
        include "68kmacros.inc"

        xdef _jag_memset32

        text
_jag_memset32: ;void jag_memset32(void *dest,uint16_t repcount,uint16_t count,uint32_t value)
        jsr     _jag_wait_blitter_ready
        move.l  4(sp),d0
        phrase_align d0,d1
        move.l	d0,A1_BASE		; dest
        move.l	d1,A1_PIXEL
        move.l	#PIXEL32|XADDPIX|PITCH1,A1_FLAGS
        move.l	16(SP),B_PATD		; value
        move.w  10(sp),d0               ; repcount
        swap    d0
        move.w  14(sp),d0               ; count (in 32bit longs)
        move.l	d0,B_COUNT
        move.l	#PATDSEL,B_CMD
        rts
