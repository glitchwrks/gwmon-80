;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMMITS1 -- GWMON-80 Small Monitor for MITS 88-2SIO
;
;This customization uses channel 0 on a MITS 88-2SIO with
;default strapping for the console.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	010H		;88-2SIO channel 0 control port
DATPRT	equ	011H		;88-2SIO channel 0 data port
STACK	equ	0C000H		;48K system

	ORG	0F800H

	INCLUDE	'sm.inc'
	INCLUDE '6850acia.inc'

	END
