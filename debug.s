*
* Simple monitor for debugging CPU exceptions.
*
* Written by Frank Wille in 2013.
* Adapted for ATARI Jaguar by Tobias Arnold in 2017.
*
*

	macro	BANNER
	dc.b	"jaglib internal debugger V1.0"
	dc.b	10
	dc.b    "press 'x' to detach Skunkboard"
	endm

NUM_VECTORS	equ	62

LINEBUFSIZE	equ	78

; from startup.s
	xref	startup

; from linker script
	xref	_BSS_END

	code

;---------------------------------------------------------------------------
	xdef	_jag_initdebug
_jag_initdebug:
; Save all exception vectors and install ours.

	; Set new vectors and redirect all exceptions to our handler.
	lea	exceptionTable(pc),a0
	move.l	#8,a1
	moveq	#NUM_VECTORS-1,d0
.1:	move.l	a0,(a1)+
	addq.l	#2,a0
	dbf	d0,.1
	rts

;---------------------------------------------------------------------------
	xdef	_jag_debug
_jag_debug:
; Direct entry into the debugger is shown as exception vector #0.
	move.w  sr,-(sp)
	subq.l  #8,sp ; manipulate the stack, so it look like an exception
	bsr.w   exception_handler
	nop

;---------------------------------------------------------------------------
exceptionTable:
	rept	NUM_VECTORS
	bsr.b	exception_handler
	endr
	nop

;---------------------------------------------------------------------------
exception_handler:
; This exception handler is called by a BSR.B from exceptionTable,
; which means that we can determine the original exception vector by
; looking at the return address.

	; save all registers
	movem.l	d0-d7/a0-a6,Registers

	; print banner
	lea	BannerTxt(pc),a0
	bsr	print

	; determine exception vector, which had been called, and print it
	lea	ExceptTxt(pc),a0
	bsr	print
	move.l	(sp)+,d7
	move.l	sp,a2			; a2 remember real SSP
	lea	exceptionTable-2(pc),a0
	sub.l	a0,d7
	lsr.w	#1,d7			; d7 Exception vector number
	move.w	d7,d0
	bsr	hexout

	; vector 2 or 3 (bus/address error) on an 68000 need special handling
	move.l	a2,a3			; a3 Exception frame
	cmp.w	#4,d7
	bhs	.1

	; 68000 saves 4 extra words before the status and PC, skip them
	addq.l	#8,a3

	; print fault PC
.1:	lea	AtPCTxt(pc),a0
	bsr	print
	move.l	2(a3),d0
	bsr	hex4out
	move.l	2(a3),d0
	bsr	showoffset

	; print SR
	lea	SRTxt(pc),a0
	bsr	print
	move.w	(a3),d0
	move.l	a2,a0
	btst	#13,d0			; save USP or SSP as A7
	bne	.2
	move	usp,a0
.2:	move.l	a0,Registers+15*4 ;(a4)
	bsr	hex2out

	; print USP
	;lea	USPTxt(pc),a0
	;bsr	print
	;move	usp,a0
	;move.l	a0,d0
	;bsr	hex4out

	; print SSP
	lea	SSPTxt(pc),a0
	bsr	print
	move.l	a2,d0
	bsr	hex4out

	bsr	newline

	; print d0-d7 and a0-a7
	bsr	printregs

	; print supervisor stack
	lea	SSTxt(pc),a0
	bsr	printstack

	; print user stack
	;lea	USTxt(pc),a0
	;move	usp,a2
	;bsr	printstack

	bsr	newline

cmd_loop:
	lea	PromptTxt(pc),a0
	bsr	print
	bsr	getline

	; check for command code
	moveq	#-$21,d0
	and.b	(a0)+,d0
.1:	cmp.b	#' ',(a0)+		; skip following blanks
	beq	.1
	subq.l	#1,a0

	sub.b	#'M',d0			; m <addr> [lines] : memory dump
	beq	memdump

	subq.b	#3,d0			; p <addr> : print offset
	beq	printoffset

	;subq.b	#1,d0			; q : quit (reboot)
	;beq	quit

	;subq.b	#1,d0			; r : register dump
	subq.b	#2,d0
	beq	regdump

	subq.b	#6,d0			; x : exit (detach skunkboard)
	beq	quit

	bra	cmd_loop

printstack:
; Print top 12 words from the stack.
; a0 = header text
; a2 = pointer to stack

	bsr	print
	moveq	#11,d2
.1:	lea	SpcTxt(pc),a0
	bsr	print
	move.w	(a2)+,d0
	bsr	hex2out
	dbf	d2,.1
	bra	newline

	cnop	0,4
quit:
;Detach the skunkboard.
	jsr	_skunkCONSOLECLOSE
.loop
	bra.s .loop

;---------------------------------------------------------------------------
memdump:
; Do a memory dump from given address.
; a0 = pointer to ASCII hex address

	; dump 8 lines by default
	moveq	#7,d2

	; read start address argument
	bsr	readhex
	move.l	d0,a2
	cmp.b	#' ',(a0)+
	bne	.lineloop

	; read optional number of lines argument
	bsr	readhex
	tst.l	d0
	beq	.lineloop
	cmp.w	#16,d0
	bhi	.lineloop
	move.w	d0,d2
	subq.w	#1,d2

.lineloop:
	; print address
	move.l	a2,d0
	bsr	hex4out
	lea	ColonTxt(pc),a0
	bsr	print

	; print 16 bytes
	moveq	#15,d3
	lea	Buffer,a3
	move.b	#$22,(a3)+
.byteloop:
	move.b	(a2)+,d0
	move.b	d0,d1
	cmp.b	#$20,d1
	blo	.1
	cmp.b	#$80,d1
	blo	.2
.1:	moveq	#'.',d1
.2:	move.b	d1,(a3)+
	bsr	hexout
	cmp.w	#8,d3
	bne	.3
	lea	DashTxt(pc),a0
	bra	.4
.3:	lea	SpcTxt(pc),a0
.4:	bsr	print
	dbf	d3,.byteloop

	; print the 16 characters (when printable)
	move.b	#$22,(a3)+
	move.b	#10,(a3)+
	clr.b	(a3)
	lea	Buffer,a0
	bsr	print
	dbf	d2,.lineloop

	bra	cmd_loop

ColonTxt:
	dc.b	": ",0
DashTxt:
	dc.b	"-",0
	even

;---------------------------------------------------------------------------
printoffset:
; Print offset to program base, for given address.
; a0 = pointer to ASCII hex address

	bsr	readhex
	bsr	showoffset
	bsr	newline
	bra	cmd_loop

;---------------------------------------------------------------------------
regdump:
; Print d0-d7, a0-a7.

	bsr	printregs
	bra	cmd_loop

;---------------------------------------------------------------------------
printregs:
; Print all registers.

	movem.l	d2/a2-a3,-(sp)

	lea	Registers,a2
	lea	RegTxt,a3
	moveq	#7,d2
	move.w	#'d0',(a3)

.1:	move.b	#'d',(a3)
	move.l	a3,a0
	bsr	print
	move.l	(a2),d0
	bsr	hex4out
	move.l	(a2),d0
	bsr	showoffset

	move.b	#'a',(a3)
	move.l	a3,a0
	bsr	print
	move.l	8*4(a2),d0
	bsr	hex4out
	move.l	8*4(a2),d0
	bsr	showoffset
	bsr	newline

	addq.w	#1,(a3)
	addq.l	#4,a2
	dbf	d2,.1

	movem.l	(sp)+,d2/a2-a3
	rts

BannerTxt:
	BANNER
	dc.b	10
LFTxt:
	dc.b	10,0
ExceptTxt:
	dc.b	"CPU Exception #",0
AtPCTxt:
	dc.b	" at"
SpcTxt:
	dc.b	" ",0
SRTxt:
	dc.b	"SR=",0
;USPTxt:
;	dc.b	" USP=",0
SSPTxt:
	dc.b	" SSP=",0
SSTxt:
	dc.b	"SS:",0
;USTxt:
;	dc.b	"US:",0
PromptTxt:
	dc.b	"> ",0
	even

;---------------------------------------------------------------------------
readhex:
; Convert a hex string into its value.
; a0 = hex string pointer
; -> a0 = new string pointer
; -> d0 = value

	moveq	#0,d0
.1:	moveq	#-$21,d1
	and.b	(a0)+,d1
	sub.b	#'0'-$20,d1
	blo	.3
	cmp.b	#9,d1
	bls	.2
	cmp.b	#'A'-('0'-$20),d1
	blo	.3
	cmp.b	#'F'-('0'-$20),d1
	bhi	.3
	sub.b	#'A'-('0'-$20)-10,d1
.2:	lsl.l	#4,d0
	or.b	d1,d0
	bra	.1
.3:	subq.l	#1,a0
	rts

;---------------------------------------------------------------------------
showoffset:
; Print offset to program start, when address is between startup and BSSEnd.
; d0 = address
; a5 = bitmap base

	move.l	d2,-(sp)
	lea		startup,a0
	move.l	d0,d2
	cmp.l	a0,d2
	blo	.1
	cmp.l	#_BSS_END,d2
	bhs	.1

	; this is a programm address, print 24-bit offset
	sub.l	a0,d2
	lea	.lpar(pc),a0
	bsr	print
	swap	d2
	move.b	d2,d0
	bsr	hexout
	rol.l	#8,d2
	move.l	d2,d0
	bsr	hexout
	rol.l	#8,d2
	move.b	d2,d0
	bsr	hexout
	lea	.rpar(pc),a0
	bsr	print
	bra	.2

	; no programm address, print empty parentheses
.1:	lea	.empty(pc),a0
	bsr	print

.2:	move.l	(sp)+,d2
	rts

.lpar:	dc.b	" (*+",0
.empty:	dc.b	" (   --   "
.rpar:	dc.b	")  ",0
	even


;---------------------------------------------------------------------------
hex4out:
; Print a hex-longword.
; d0 = longword
; a5 = bitmap base

	move.l	d2,-(sp)
	rol.l	#8,d0
	move.l	d0,d2
	bsr	hexout
	rol.l	#8,d2
	move.b	d2,d0
	bsr	hexout
	rol.l	#8,d2
	move.b	d2,d0
	bsr	hexout
	rol.l	#8,d2
	move.b	d2,d0
	bsr	hexout
	move.l	(sp)+,d2
	rts


;---------------------------------------------------------------------------
hex2out:
; Print a hex-word.
; d0 = word
; a5 = bitmap base

	move.l	d2,-(sp)
	move.b	d0,d2
	lsr.w	#8,d0
	bsr	hexout
	move.b	d2,d0
	bsr	hexout
	move.l	(sp)+,d2
	rts


;---------------------------------------------------------------------------
hexout:
; Print a hex-byte.
; d0 = byte
; a5 = bitmap base

	move.l	#$30300000,-(sp)	; "00\0\0"
	moveq	#15,d1
	and.b	d0,d1
	cmp.b	#10,d1
	blo	.1
	add.b	#'A'-'0'-10,d1
.1:	lsl.w	#4,d0
	and.w	#$0f00,d0
	cmp.w	#$0a00,d0
	blo	.2
	add.w	#('A'-'0'-10)<<8,d0
.2:	move.b	d1,d0
	add.w	d0,(sp)
	move.l	sp,a0
	bsr	print
	addq.l	#4,sp
	rts


;---------------------------------------------------------------------------
newline:
; Move cursor to the beginning of the next line.
; a5 = bitmap base

	lea	LFTxt(pc),a0


;---------------------------------------------------------------------------
	xref _stderr
	xref _fprintf
print:
; Write a string via skunkboard.
; a0 = null-terminated string, '\n' starts a new line
	movem.l	.l3,-(a7)
	move.l	a0,-(a7)
	move.l	_stderr,-(a7)
	jsr	_fprintf
	addq.w	#8,a7
.l3	reg
	rts

;---------------------------------------------------------------------------
	xref _skunkCONSOLEREAD
	xref _skunkCONSOLECLOSE

getline:
; Read a line from the keyboard. Return pointer to input.
; a5 = bitmap base
; -> a0 = input buffer, terminated by zero
	lea Buffer,a0
	move.l #LINEBUFSIZE,d0
	jsr _skunkCONSOLEREAD
	lea Buffer,a0
	rts

	section	data

RegTxt:
	dc.b	"d0=",0
	even

	section	bss


	; registers before entering the exception
Registers:
	ds.l	16

	; line input buffer
Buffer:
	ds.b	LINEBUFSIZE
