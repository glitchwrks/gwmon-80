;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMMITS2 -- GWMON-80 Small Monitor for MITS 88-SIO rev 1
;
;This customization works with the old MITS 88-SIO rev 1
;board, which uses a single 40-pin UART (e.g. TR1402 
;compatible) and comes in A, B, and C varieties:
;
; * 88-SIO-A uses RS-232 levels
; * 88-SIO-B uses TTL levels
; * 88-SIO-C uses 20 mA current loops
;
;All three variants present the same UART interface and are
;basically identical except for the external interface.
;
;Default strapping is assumed. Bitrate is set with prescaler
;jumpers. BEWARE! The MITS 88-SIO rev 1 is *very* different
;from the 88-SIO rev 0 w.r.t. programming!
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	00H		;88-SIO control port
DATPRT	equ	01H		;88-SIO data port
STACK	equ	0C000H		;48K system

	ORG	0F800H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE 'scmdnull.inc'	;Command table terminator

	INCLUDE 'mitssio1.inc'	;MITS 88-SIO rev 1 I/O

	END
