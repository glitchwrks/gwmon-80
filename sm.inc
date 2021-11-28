;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SM -- GWMON-80 Small Monitor
;
;This is the small monitor for GWMON-80. It includes the
;small command processor and common monitor routines.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Small Monitor Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DYNOUT	equ	STACK - 3
DYNIN	equ	STACK - 6

	INCLUDE	'vectors.inc'

CSTART:	LXI	SP, STACK	;Set up stack pointer

	LXI	H, 0C9D3H	;Prime area above "real" stack
	PUSH	H		;for dynamic IN and OUT
	LXI	D, 0DBC9H
	PUSH	D
	PUSH	H

	CALL	IOSET		;Do I/O module setup
	LXI	H, SIGNON$	;SM signon
	CALL	PRTSTR
WSTART:

	INCLUDE	'scp.inc'
	INCLUDE	'common.inc'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Small Monitor Strings
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SIGNON$:	db CR, LF, LF, 'GWMON-80 0.9 SM'
CSERR$:		db 'CHECKSUM '
ERR$:		db 'ERROR', 0
