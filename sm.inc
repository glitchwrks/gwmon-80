;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SM -- GWMON-80 Small Monitor
;
;This is the small monitor for GWMON-80. It includes the
;small command processor and common monitor routines.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Small Monitor Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DYNIN	equ	STACK - 3
DYNOUT	equ	STACK - 6

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;SMSTRT -- SM cold start routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SMSTRT:	LXI	SP, STACK	;Set up stack pointer

	LXI	H, 0C9D3H	;Prime area above "real" stack
	PUSH	H		;for dynamic IN and OUT
	LXI	D, 0DBC9H
	PUSH	D
	PUSH	H

	CALL	IOSET		;Do I/O module setup
	LXI	H, SIGNON$	;SM signon
	CALL	PRTCLS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;WSTART -- Warm start routine
;
;Falls through to the command processor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
WSTART: LXI	SP, STACK - 6	;Reload stack pointer, don't clobber
				;dynamic IN/OUT area

	INCLUDE	'scp.inc'
	INCLUDE	'common.inc'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Small Monitor Strings
;
;These strings are terminated by setting the high bit in the
;last character.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SIGNON$:	db 'GWMON-80 0.9 S', 'M' + 80H
PROMPT$:	db LF, '>' + 80H
CSERR$:		db 'CKSUM '
ERR$:		db 'ERRO', 'R' + 80H
