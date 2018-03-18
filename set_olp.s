    include "jaguar.inc"

    xdef _jag_set_olp
    xref __global_olp

    text
_jag_set_olp:
    swap    d0
    move.l  d0,__global_olp
    rts
