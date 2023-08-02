;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMMITS3 -- GWMON-80 Small Monitor for MITS Turnkey board
;
;This customization uses RAM, ROM, and the serial channel
;on a MITS Turnkey board with default switch settings. It 
;can be burned to two 1702A EPROMs and placed in the two
;leftmost sockets.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	010H		;Turnkey ACIA ccontrol port
DATPRT	equ	011H		;Turnkey ACIA data port
STACK	equ	0FC00H		;Use Turnkey board 1K RAM

	ORG	0FC00H		;Assemble for first two ROMs

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE 'scmdnull.inc'	;Command table terminator

	INCLUDE '6850acia.inc'	;Generic Motorola 6850 ACIA I/O

	END
