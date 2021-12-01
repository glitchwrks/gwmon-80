;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;XM -- GWMON-80 Extended Monitor
;
;This is the extended monitor for GWMON-80. It includes the
;extended command processor and common monitor routines.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Extended Monitor Equates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STACK	equ	TOPMEM - 0100H	;Reserve 256 bytes for buffer, vars
DYNOUT	equ	STACK - 3	;Dynamic OUT instruction area
DYNIN	equ	STACK - 6	;Dynamic IN instruction area
INBUFF	equ	TOPMEM - 0100H	;Console input buffer
MAXBUF	equ	32		;Console input buffer max size

	INCLUDE	'vectors.inc'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;XMSTRT -- XM cold start routine
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
XMSTRT:	LXI	SP, STACK	;Set up stack pointer

	LXI	H, 0C9D3H	;Prime area above "real" stack
	PUSH	H		;for dynamic IN and OUT
	LXI	D, 0DBC9H
	PUSH	D
	PUSH	H

	XRA	A		;Set up the NULL command area
	LXI	H, NULCMD	;which terminates the command list
	MVI	C, 16		;and allows for expansion
XMSTR1:	MOV	M, A
	DCR	C
	INX	H
	JNZ	XMSTR1

	CALL	IOSET		;Do I/O module setup
	LXI	H, SIGNON$	;SM signon
	CALL	PRTSTR
WSTART:

	INCLUDE	'xcp.inc'
	INCLUDE	'common.inc'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Small Monitor Strings
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SIGNON$:	db 'GWMON-80 0.9 X','M' + 80H
PROMPT$:	db LF, 'XM', '>' + 80H
CSERR$:		db 'CHECKSUM '
ERR$:		db 'ERRO', 'R' + 80H
