; TESTING
; cl65 -t none -C nes.cfg -o test.nes test.s

.segment "CODE"

reset:
   sei
   lda #$FF
loop:
   jmp loop
   rti

nmi:
   rti

irq:
   rti

.segment "HEADER"
   .byte "NES" ; signature
   .byte $1A   ; signature
   .byte $02   ; # of 16kb PRG-ROM banks
   .byte $00   ; # of 8kb VROM banks
   .byte $00   ; ROM control byte one
   .byte $00   ; ROM control byte two
   .byte $00   ; # of 8kb RAM banks
   .byte $00   ; reserved

.segment "VECTORS"
   .addr nmi
   .addr reset
   .addr irq