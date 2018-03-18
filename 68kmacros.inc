	macro phrase_align \1, \2
        move.l  \1,\2
        andi.l  #~7,\1   ; phrase align
        andi.l  #7,\2          ; index
	endm
