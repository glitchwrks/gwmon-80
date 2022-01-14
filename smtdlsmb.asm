;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMTDLSMB -- GWMON-80 Small Monitor for TDL SMB
;
;This customization uses the TTY ACIA and onboard 2K of RAM
;on a Technical Design Labs (TDL) System Monitor Board (SMB)
;
;No offboard RAM is required for operation.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Hardware Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CTLPRT	equ	070H		;TTY ACIA control port
DATPRT	equ	071H		;TTY ACIA data port
STACK	equ	0F900H		;Stack on first page of SMB RAM

	ORG	0F000H

	INCLUDE	'vectors.inc'	;Standard GWMON-80 jump table

	INCLUDE	'sm.inc'	;The small monitor
	INCLUDE	'scmdstd.inc'	;SM standard commands
	INCLUDE 'scmdnull.inc'	;Command table terminator

	INCLUDE '6850acia.inc'	;Generic Motorola 6850 ACIA I/O

	END
