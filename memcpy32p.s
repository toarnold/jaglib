        include "jaguar.inc"
        include "68kmacros.inc"
        
        xdef _jag_memcpy32p
        xref _jag_wait_blitter_ready

        text
        ; _jag_memcpy32p uses 32bit blitter phrase mode.
        ; dest and src must be phrase aligned
_jag_memcpy32p: ;void _jag_memcpy32p(void *dest,void *src,uint16_t repcount,uint16_t longcount)
        jsr     _jag_wait_blitter_ready
        move.l  4(sp),A1_BASE

        move.l  #0,A1_PIXEL
        move.l  #PIXEL32|XADDPHR|PITCH1,A1_FLAGS
        move.l  8(sp),A2_BASE
        
        move.l  #0,A2_PIXEL         ;; start in front
        move.l  #PIXEL32|XADDPHR|PITCH1,A2_FLAGS ; use 32bit pixels/phrasemode
        move.w  14(sp),d0
        swap    d0
        move.w  18(sp),d0
        move.l	d0,B_COUNT
        move.l  #LFU_REPLACE|SRCEN,B_CMD
        rts
