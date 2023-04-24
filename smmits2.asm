;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMMITS2 -- GWMON-80 Small Monitor for MITS 88-SIO
;
;This customization works with the old MITS 88-SIO board,
;which uses a single 40-pin UART (e.g. TR1402 compatible)
;and comes in A, B, and C varieties:
;
; * 88-SIOA uses RS-232 levels
; * 88-SIOB uses TTL levels
; * 88-SIOC uses 20 mA current loops
;
;All three variants present the same UART interface and are
;basically identical except for the external interface.
;
;Default strapping is assumed. Bitrate is set with prescaler
;jumpers. BEWARE! The MITS 88-SIO manual contains errors in
;the bitrate table!
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

	INCLUDE 'mitssio.inc'	;MITS 88-SIO I/O

	END
