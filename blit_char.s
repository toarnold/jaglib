        include "jaguar.inc"
        include "68kmacros.inc"

        xdef _jag_blit_char
        xdef _jag_xlfont

        xref _jag_vidmem
        xref _jag_wait_blitter_ready

        text

_jag_blit_char:						; void jag_blit_char(byte *src,uint16_t offset,uint8_t color_index)
        jsr     _jag_wait_blitter_ready
        move.l	_jag_vidmem,A1_BASE	;destAdr
        move.l	8(sp),A1_PIXEL		; offset
        move.l	#PIXEL8|XADDPIX|WID320|PITCH1,A1_FLAGS
        move.l	#$0001FFF8,A1_STEP
        move.l	4(sp),A2_BASE		; src
        move.l	#0,A2_PIXEL
        move.l	#PIXEL1|XADDPIX|WID8|PITCH1,A2_FLAGS
        move.l	#$0001ffff,A2_STEP
        move.l	#$00080008,B_COUNT
        move.l  12(sp),B_PATD
        move.l	#253,B_DSTD
        move.l	#SRCENX|UPDA1|UPDA2|BCOMPEN|BKGWREN|PATDSEL,B_CMD
        rts

        DATA

        cnop 0,8
_jag_xlfont:
    	incbin "XLFontASCIIorder.raw"
